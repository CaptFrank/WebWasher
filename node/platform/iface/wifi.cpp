/*
 * wifi.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: fpapinea
 */

#include "wifi.h"
#include <configs.h>

/**
 * @brief The Wifi Interface Constructor
 *
 * This is the entry method for the Wifi interface object.
 * We must pass the stack instance and the wifi settings
 * strucutures.
 *
 * @param stack		The stack instance
 * @param settings	The wifi settings strucutre
 */
wifi::wifi(WifiIPStack* stack, wifi_attributes_t* settings){

	/*
	 * Assign the stack pointer to our own pointer
	 */
	stack_if = stack;

	/*
	 * Assign the wifi_settings internally
	 */
	attr = settings;

	/*
	 * We set the configs
	 */
	attr->handle->config(
			*settings->ip,
			*settings->dns,
			*settings->gateway,
			*settings->subnet);

	/*
	 * Set the internals
	 */
	wifi_if = settings->handle;

	/*
	 * Get the firmware versions
	 */
	attr->drv_ver = wifi_if->driverVersion();
	attr->fw_ver = wifi_if->firmwareVersion();

	/*
	 * Check the internal pointers.
	 */
	if(stack_if
			&& attr
			&& wifi_if){

		/*
		 * Set the status to ok
		 */
		status = STATUS_OK;
	}else{

		/*
		 * There is a problem
		 */
		status = ERR_IO_ERROR;
	}
}

/**
 * @brief The Connect method for connecting to the wifi AP
 *
 * This method connects this node to a wifi AP based on the
 * SSID past and th epast password. We should return true or false
 * base don th esuccess of the connection.
 *
 * @param ssid		The SSID of the wifi AP
 * @param pass		The password of the wifi AP
 * @return bool		True if success, False if failed
 */
wl_status_t wifi::connect(wifi_ssid_t ssid, wifi_pass_t pass){

	/*
	 * Initialize the connection
	 */
	PRINT("Connecting to wifi: ");
	PRINTLN(ssid);

	wifi_if->begin(ssid, pass);

	/*
	 * Wait for an IP
	 */
	while(wifi_if->localIP() == INADDR_NONE){
		PRINT(".");
		delay(300);
	}

	/*
	 * We have an ip
	 */
	PRINTLN();
	PRINT("Ip Address obtained: ");
	PRINTLN(wifi_if->localIP());

	/*
	 * Return the status
	 */
	return wifi_if->WiFi_status;
}

/**
 * @brief The Disconnection mechansim for the wifi conenction.
 *
 * This method disconnects the node from the AP and returns
 * a bool representing the success of the disconnection.
 *
 * @return bool		True if success, False if failed
 */
wl_status_t wifi::disconnect(){
	return (wl_status_t)wifi_if->disconnect();
}

/**
 * @brief Start scan WiFi networks available
 *
 * @return int8_t	Number of discovered networks
 */
int8_t wifi::scan_networks(){
	return wifi_if->scanNetworks();
}

/**
 * @brief Gets the internal wifi interface class.
 *
 * @return handle	The wifi class handle
 */
WiFiClass* wifi::get_if(){
	return wifi_if;
}

/**
 * @brief Gets the wifi internal stack handle.
 *
 * @return handle	The wifi handler stack handle.
 */
WifiIPStack* wifi::get_stack(){
	return stack_if;
}
