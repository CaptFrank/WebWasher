/*
 * coms.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: fpapinea
 */

#include <service/services/coms.h>

/**
 * This is the command map that will generate the command type.
 */
static command_map_t commands[COMMAND_TYPE_MAX_SIZE + 1] = {

		/*
		 * Control
		 */
		{ MQTT_SUB_COMMAND_REBOOT, 		COMMAND_TYPE_REBOOT 	},
		{ MQTT_SUB_COMMAND_RESTART, 	COMMAND_TYPE_RESTART	},
		{ MQTT_SUB_COMMAND_SUSPEND, 	COMMAND_TYPE_SUSPEND	},
		{ MQTT_SUB_COMMAND_SELFTEST, 	COMMAND_TYPE_SELFTEST	},

		/*
		 * Requests
		 */
		{ MQTT_SUB_COMMAND_STATUS, 		COMMAND_TYPE_STATUS		},
		{ MQTT_SUB_COMMAND_GET, 		COMMAND_TYPE_GET		},
};

/**
 * @brief The default constructor for the comms object
 *
 * This is where we initialize the interfaces; both the
 * MQTT interface and the underlying wifi interface.
 */
coms_svr::coms_svr(){

	/*
	 * Set the wifi attributes
	 */
	wifi_attr.handle 	= new WiFiClass();

	wifi_attr.ip 		= WIFI_LOCAL_IP;
	wifi_attr.dns 		= WIFI_LOCAL_DNS;
	wifi_attr.gateway 	= WIFI_LOCAL_GATEWAY;
	wifi_attr.subnet 	= WIFI_LOCAL_SUBNET;

	/*
	 * Create the ip stack handle
	 */
	ip_stack = new WifiIPStack();

	/*
	 * Create the wifi iface
	 */
	wifi_if = new wifi_t(ip_stack, &wifi_attr);

	/*
	 * Create the mqtt iface
	 */
	mqtt_if = new mqtt_t(ip_stack);
}

/**
 * @brief Connects to the wifi hotspot and to the mqtt broker.
 *
 * The default entry point to connect both to the wifi hotspot and
 * to the mqtt broker.
 *
 * @param 	iface			The index of the interface to operate on
 * @return 	status			The status of the operation
 */
status_code_t coms_svr::connect(interface_t iface){

	/*
	 * We switch on the interface type
	 */
	switch(iface){

		/*
		 * Connect MQTT
		 */
		case INTERFACE_MQTT:

			// Check
			if(MQTT_CONNECTED != \
					mqtt_if->connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT)){

				/*
				 * Problem
				 */
				goto MQTT_ERROR;
			}else{

				/*
				 * Subscribe to the topics of interest
				 */
				if(MQTT_SUCCESS_STATUS != \
						mqtt_if->subscribe(MQTT_SUB_TOPIC_1, coms_t::coms_callback)){

					/*
					 * Problem
					 */
					goto MQTT_ERROR;
				}
				if(MQTT_SUCCESS_STATUS != \
						mqtt_if->subscribe(MQTT_SUB_TOPIC_2, coms_t::coms_callback)){

					/*
					 * Problem
					 */
					goto MQTT_ERROR;
				}			}
			return STATUS_OK;

		/*
		 * Connect WIFI
		 */
		case INTERFACE_WIFI:

			// Check
			if(WL_CONNECTED != \
					wifi_if->connect(WIFI_SSID, WIFI_PASS)){

				/*
				 * Problem
				 * We should abort the connection and report
				 */
				system_t::BIOS_alert(BIOS_ALERT_COMS_FAIL);
				system_t::BIOS_reboot(BIOS_REBOOT_WIFI);
				return ERR_IO_ERROR;
			}
			return STATUS_OK;

		/*
		 * Connect both WIFI and MQTT
		 */
		case INTERFACE_BOTH:

			// Check
			if((WL_CONNECTED != \
					wifi_if->connect(WIFI_SSID, WIFI_PASS)) &&
				MQTT_CONNECTED != \
					mqtt_if->connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT)){

				/*
				 * Problem
				 * We should abort the connection and report
				 */
				system_t::BIOS_alert(BIOS_ALERT_COMS_FAIL);
				system_t::BIOS_reboot(BIOS_REBOOT_WIFI);
				system_t::BIOS_reboot(BIOS_REBOOT_MQTT);
				return ERR_IO_ERROR;
			}
			return STATUS_OK;

		default:
			return STATUS_OK;
	}

	MQTT_ERROR:

	/*
	 * Problem
	 * We should abort the connection and report
	 */
	system_t::BIOS_alert(BIOS_ALERT_COMS_FAIL);
	system_t::BIOS_reboot(BIOS_REBOOT_MQTT);
	return ERR_IO_ERROR;
}

/**
 * @brief Disconnects from the wifi hotspot and the mqtt broker.
 *
 * The default entry point to connect both to the wifi hotspot and
 * to the mqtt broker.
 *
 * @param 	iface			The index of the interface to operate on
 * @return 	status			The status of the operation
 */
status_code_t coms_svr::disconnect(interface_t iface){

	/*
	 * We switch on the interface type
	 */
	switch(iface){

		/*
		 * Disconnect the MQTT interface
		 */
		case INTERFACE_MQTT:

			// Check
			if(MQTT_SUCCESS_STATUS != mqtt_if->disconnect()){

				/*
				 * Problem
				 */
				goto DISCONNECT_ABORT;
			}
			return STATUS_OK;

		/*
		 * Disconnect the WIFI interface
		 */
		case INTERFACE_WIFI:
			wifi_if->disconnect();
			return STATUS_OK;

		/*
		 * Disconnect both interfaces
		 */
		case INTERFACE_BOTH:
			wifi_if->disconnect();

			// Check
			if(MQTT_SUCCESS_STATUS != mqtt_if->disconnect()){

				/*
				 * Problem
				 */
				goto DISCONNECT_ABORT;
			}
			return STATUS_OK;

		default:
			return STATUS_OK;
	}

	/*
	 * Abort the connection
	 */
	DISCONNECT_ABORT:

	/*
	 * Problem
	 * We should abort the connection and report
	 */
	system_t::BIOS_alert(BIOS_ALERT_COMS_FAIL);
	system_t::BIOS_reboot(BIOS_REBOOT_MQTT);
	return ERR_IO_ERROR;
}

/**
 * @brief Sends a message to the broker
 *
 * This sends a message type to the mqtt broker or to the receiving server.
 *
 * @param 	msg_type		The message type
 * @param 	msg				The message data
 * @return 	status			The status of the operation
 */
mqtt_status_t coms_svr::send(msg_type_t msg_type, msg_t* msg){

	// Temporary message
	MQTT::Message packet;

	/*
	 * Create the attributes
	 */
	packet.payload 		= msg->data;
	packet.payloadlen 	= msg->length;

	/*
	 * We switch on the message type
	 */
	switch(msg_type){

		/*
		 * We send out the accelerometer data
		 */
		case MSG_TYPE_ACC_DATA:

			/*
			 * Send the string
			 */
			return mqtt_if->publish(MQTT_PUBLISH_DATA_ACC, &packet);

		/*
		 * We send out the temperature data
		 */
		case MSG_TYPE_TEMP_DATA:

			/*
			 * Send the string
			 */
			return mqtt_if->publish(MQTT_PUBLISH_DATA_TEMP, &packet);

		/*
		 * We send out the status
		 */
		case MSG_TYPE_STATUS:

			/*
			 * Send the string
			 */
			return mqtt_if->publish(MQTT_PUBLISH_STATUS, &packet);

		/*
		 * Mostly used for echoe
		 */
		case MSG_TYPE_OTHER:

		/*
		 * We send out the heartbeat
		 */
		case MSG_TYPE_HEARTBEAT:
		default:

			/*
			 * Sending the hearbeat is through a different
			 * socket than the mqtt service socket
			 */
			return ip_stack->write(msg->data, msg->length, WIFI_DEFAULT_TIMEOUT);
			break;
	}
}

/**
 * @brief Processes a request
 *
 * This processes a received message from the mqtt borker.
 *
 * @param 	command			The command type received
 * @param	msg				The message data
 * @return 	status			The status of the operation
 */
status_code_t coms_svr::process(command_t command, msg_t* msg){

	/*
	 * Status
	 */
	status_code_t rc;

	/*
	 * Get the message
	 */
	uint8_t* packet = malloc(msg->length);
	memcpy(packet, msg->data, msg->length);

	/*
	 * We switch on the command type
	 */
	switch(command){

		/*
		 * Disconnect interfaces
		 */
		case COMMAND_TYPE_DISCONNECT:
			rc = disconnect((interface_t)*packet);
			free(packet);
			return rc;

		/*
		 * Reboot a selected service/driver
		 */
		case COMMAND_TYPE_REBOOT:
			system_t::BIOS_reboot((bios_reboot_t)*packet);
			free(packet);
			return rc;

		/*
		 * Request a status update
		 */
		case COMMAND_TYPE_STATUS:
			rc = send(MSG_TYPE_STATUS, NULL);
			free(packet);
			return rc;

		/*
		 * Restart the OS
		 */
		case COMMAND_TYPE_RESTART:
			system_t::BIOS_reboot(BIOS_REBOOT_OS);
			free(packet);
			return rc;

		/*
		 * Suspend the specified task/OS
		 */
		case COMMAND_TYPE_SUSPEND:

			/*
			 * Send the suspend request to the system queue
			 */
			system_t::BIOS_suspend((thread_id_t)*id);
			free(packet);
			return rc;

		/*
		 * Request a selftest of the device
		 */
		case COMMAND_TYPE_SELFTEST:
			system_t::BIOS_selftest();
			free(packet);
			return rc;

		/*
		 * Request a get of data
		 */
		case COMMAND_TYPE_GET:
			rc = send((msg_type_t)(*packet)[0], NULL);
			free(packet);
			return rc;

		/*
		 * Echo out the data sent to the device
		 */
		case COMMAND_TYPE_ECHO:
			rc = send(MSG_TYPE_OTHER, packet);
			free(packet);
			return rc;

		/*
		 * No request sent with substance, we sned a heartbeat.
		 */
		default:
			rc = send(MSG_TYPE_HEARTBEAT, NULL);
			free(packet);
			return rc;

	}
}

/**
 * @brief The callback for a subscription
 *
 * This is the callback for the topic subscription.
 * It is invoked when there is a message that corresponds
 * to a topic specified is received.
 *
 * @param md				The message descriptor
 */
void coms_svr::coms_callback(MQTT::MessageData& md){

	// Index
	uint8_t index = 0;

	// Token holder
	char* token;
	char* previous;

	// Packet
	msg_t packet;

	/*
	 * We do some simple processing to the message before
	 * sending it to the coms service.
	 */

	/*
	 * Tokenize the string to get last object
	 */
	do{
		previous 	= token;
		token 		= strtok(topic, "/");
	}while(token != NULL);

	/*
	 * Compare the topic
	 */
	for(uint8_t i = 0; i < COMMAND_TYPE_MAX_SIZE; i ++){

		/*
		 * Compare the topic and issue the process request
		 */
		if(strcmp(commands[i].topic, previous) == COMPARE_SUCCESS){

			/*
			 * Create a message object
			 */
			packet.data 	= (void*)payload;
			packet.length 	= length;

			/*
			 * Process that type of message
			 */
			if(coms_t::process(commands[i].type, &packet) != STATUS_OK){

				/*
				 * There has been a problem with the processing.
				 * We should reboot the mqtt interface.
				 */
				system_t::BIOS_alert(BIOS_ALERT_PROCESSING_FAIL);
				system_t::BIOS_reboot(BIOS_REBOOT_MQTT);
			}
		}
	}
}

/*
 * ===================================================================
 * Default callback
 */

/**
 * @brief The default callback function for the MQTT engine.
 *
 * 	This method is called when there is a new message that is received
 * 	from the mqtt interface.
 *
 * @param topic			The topic that the message is received in
 * @param payload		The actual payload of the message
 * @param length		The length of the message
 */
void callback(char* topic, byte* payload, unsigned int length){

	/*
	 * Message temp container
	 */
	char* msg 			= 0;
	uint16_t counter	= 0;

	/*
	 * Create the message container
	 */
	msg = (char*)malloc(length * sizeof(char));

	/*
	 * Copy the message to the container
	 */
	for(count = 0; count < length; count++) {

		/*
		 * Copy the data over
		 */
		msg[count] = payload[count];
	}

	/*
	 * Activate the received message LED
	 */
	if(length > 0) {

		/*
		 * Notify the USER
		 */
		digitalWrite(RECEIVE_LED, HIGH);
		delay(1000);
		digitalWrite(RECEIVE_LED, LOW);
	}

	/*
	 * Free the heap
	 */
	free(msg);
}
