/*
 * coms.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: fpapinea
 */

#include <service/services/coms/coms.h>
#include <service/services/formatter/formatter.h>
#include <service/services/system/system.h>

using namespace system_base;

/**
 * @brief The default constructor for the comms object
 *
 * This is where we initialize the interfaces; both the
 * MQTT interface and the underlying wifi interface.
 */
coms_service::coms_service(){

	/*
	 * Set the wifi settings
	 */
	wifi_attrs.dns 		= new IPAddress((const uint8_t*)WIFI_LOCAL_DNS);
	wifi_attrs.gateway 	= new IPAddress((const uint8_t*)WIFI_LOCAL_GATEWAY);
	wifi_attrs.subnet 	= new IPAddress((const uint8_t*)WIFI_LOCAL_SUBNET);
	wifi_attrs.ip 		= new IPAddress((const uint8_t*)WIFI_LOCAL_IP);
	wifi_attrs.handle	= &WiFi;

	/*
	 * Create the wifi interface
	 */
	wifi_if = new wifi_t(&wifi_attrs);


	/*
	 * Set the mqtt params
	 */
	mqtt_params.server_name = (char*)MQTT_BROKER_ADDR;
	mqtt_params.server_port = MQTT_BROKER_PORT;
	mqtt_params.user 		= (char*)MQTT_USERNAME;

	/*
	 * Create the mqtt iface
	 */
	mqtt_if = new mqtt_t(&wifi_stack, &mqtt_params);
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
status_code_t coms_service::connect(interface_t iface){

	/*
	 * We switch on the interface type
	 */
	switch(iface){

		/*
		 * Connect MQTT
		 */
		case INTERFACE_MQTT:

			// Check
			if(MQTT_CONNECTED != mqtt_if->connect()){

				/*
				 * Problem
				 */
				goto MQTT_ERROR;
			}else{

				/*
				 * Build a struct
				 */
				mqtt_callback_entry_t entry;
				entry.context = this;

				entry.topic = (char*)MQTT_SUB_TOPIC_1;
				entry.callback = (mqtt_action_t)coms_service::__process_local;

				/*
				 * Subscribe to the topics of interest
				 */
				if(MQTT_SUCCESS_STATUS != \
						mqtt_if->subscribe((char*)MQTT_SUB_TOPIC_1, &entry)){

					/*
					 * Problem
					 */
					goto MQTT_ERROR;
				}

				entry.topic = (char*)MQTT_SUB_TOPIC_1;
				entry.callback = (mqtt_action_t)coms_service::__process_local;

				if(MQTT_SUCCESS_STATUS != \
						mqtt_if->subscribe((char*)MQTT_SUB_TOPIC_2, &entry)){

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
				system_base::BIOS_alert(BIOS_ALERT_COMS_FAIL);
				system_base::BIOS_reboot(BIOS_REBOOT_WIFI);
				return ERR_IO_ERROR;
			}
			return STATUS_OK;

		/*
		 * Connect both WIFI and MQTT
		 */
		case INTERFACE_BOTH:

			// Check
			if((ERR_IO_ERROR != \
					this->connect(INTERFACE_WIFI)) &&
				ERR_IO_ERROR != \
					this->connect(INTERFACE_MQTT)){

				/*
				 * Problem
				 * We should abort the connection and report
				 */
				system_base::BIOS_alert(BIOS_ALERT_COMS_FAIL);
				system_base::BIOS_reboot(BIOS_REBOOT_WIFI);
				system_base::BIOS_reboot(BIOS_REBOOT_MQTT);
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
	system_base::BIOS_alert(BIOS_ALERT_COMS_FAIL);
	system_base::BIOS_reboot(BIOS_REBOOT_MQTT);
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
status_code_t coms_service::disconnect(interface_t iface){

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

			// Check
			if(WL_DISCONNECTED != wifi_if->disconnect()){

				/*
				 * Problem
				 */
				goto DISCONNECT_ABORT;
			}
			return STATUS_OK;

		/*
		 * Disconnect both interfaces
		 */
		case INTERFACE_BOTH:

			// Check
			if(ERR_IO_ERROR != this->disconnect(INTERFACE_MQTT)){

				/*
				 * Problem
				 */
				goto DISCONNECT_ABORT;
			}
			// Check
			if(ERR_IO_ERROR != this->disconnect(INTERFACE_WIFI)){

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
	system_base::BIOS_alert(BIOS_ALERT_COMS_FAIL);
	system_base::BIOS_reboot(BIOS_REBOOT_MQTT);
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
mqtt_status_t coms_service::send(msg_type_t msg_type, msg_t* msg){

	/*
	 * Temporary message type
	 */
	mqtt_message_t message;

	/*
	 * Build the message
	 */
	message.length = msg->length;
	memcpy(message.payload, msg->data, msg->length);
	message.retained = true;

	/*
	 * We switch on the message type
	 */
	switch(msg_type){

		/*
		 * We send out the accelerometer data
		 */
		case MSG_TYPE_ACC_DATA:
			message.topic = (mqtt_topic_t)MQTT_PUBLISH_DATA_ACC;
			break;
		/*
		 * We send out the temperature data
		 */
		case MSG_TYPE_TEMP_DATA:
			message.topic = (mqtt_topic_t)MQTT_PUBLISH_DATA_TEMP;
			break;

		/*
		 * We send out the status
		 */
		case MSG_TYPE_STATUS:
			message.topic = (mqtt_topic_t)MQTT_PUBLISH_STATUS;
			break;

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
			return (mqtt_status_t)wifi_stack.write((uint8_t*)msg->data, msg->length);

		/*
		 * Write the data to the mqtt socket
		 */
		return mqtt_if->publish(&message);
	}
}

/**
 * @brief Processes a local request
 *
 * This processes a received message from the mqtt borker.
 *
 * @param obj		The object to use
 * @param payload	The payload array
 * @param length 	The payload length
 * @return status	The status of the operation
 */
status_code_t coms_service::__process_local(void* obj, void* payload, uint16_t length){

	/*
	 * Cast the object to a coms_service object
	 */
	coms_service* coms = (coms_service*)obj;

	/*
	 * Status
	 */
	status_code_t rc;

	/*
	 * Get the message
	 */
	uint8_t* buf = (uint8_t*)malloc(length);
	memcpy(buf, payload,  length);

	/*
	 * Convert the byte array to a packet type
	 */
	packet_t* packet = (packet_t*) buf;

	/*
	 * We switch on the command type
	 */
	switch(packet->components.command){

		/*
		 * Disconnect interfaces
		 */
		case COMMAND_TYPE_DISCONNECT:
		{
			if(length > 1) return ERR_BAD_FORMAT;
			rc = coms->disconnect((interface_t)(packet->components.argument));
			free(packet);
			break;
		};

		/*
		 * Reboot a selected service/driver
		 */
		case COMMAND_TYPE_REBOOT:
		{
			if(length > 1) return ERR_BAD_FORMAT;
			system_base::BIOS_reboot((bios_reboot_t)(packet->components.argument));
			free(packet);
			break;
		};

		/*
		 * Request a status update
		 */
		case COMMAND_TYPE_STATUS:
		{
			// Format the message

			msg_t* json = formatter_t::format(CACHE_TYPE_STATUS);
			rc = (status_code_t)coms->send(MSG_TYPE_STATUS, json);
			free(packet);
			break;
		};

		/*
		 * Restart the OS
		 */
		case COMMAND_TYPE_RESTART:
		{
			system_base::BIOS_reboot(BIOS_REBOOT_OS);
			free(packet);
			break;
		};

		/*
		 * Suspend the specified task/OS
		 */
		case COMMAND_TYPE_SUSPEND:
		{
			if(length > 1) return ERR_BAD_FORMAT;

			/*
			 * Send the suspend request to the system queue
			 */
			system_base::BIOS_suspend((thread_id_t)(packet->components.argument));
			free(packet);
			break;
		};

		/*
		 * Request a selftest of the device
		 */
		case COMMAND_TYPE_SELFTEST:
		{
			NOTIFY_INFO("Selftest - Not supported.");
			free(packet);
			break;
		};

		/*
		 * Request a get of data
		 */
		case COMMAND_TYPE_GET:
		{
			if(length > 1) return ERR_BAD_FORMAT;
			rc = (status_code_t)coms->send((msg_type_t)(packet->components.argument), NULL);
			free(packet);
			break;
		};

		/*
		 * Echo out the data sent to the device
		 */
		case COMMAND_TYPE_ECHO:
		{
			rc = (status_code_t)coms->send(MSG_TYPE_OTHER, (msg_t*)packet->components.argument);
			free(packet);
			break;
		};

		/*
		 * No request sent with substance, we sned a heartbeat.
		 */
		default:
		{
			rc = (status_code_t)coms->send(MSG_TYPE_HEARTBEAT, NULL);
			free(packet);
			break;
		};
	}
	return rc;
}
