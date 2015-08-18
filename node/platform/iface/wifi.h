/*
 * wifi.h
 *
 *  Created on: Jul 10, 2015
 *      Author: fpapinea
 */

#ifndef SERVICES_COMMS_WIFI_H_
#define SERVICES_COMMS_WIFI_H_

#include <Wifi.h>
#include <WifiIPStack.h>


/**
 * @brief typedefs
 */
typedef char* 			wifi_ssid_t;
typedef char* 			wifi_pass_t;
typedef IPAddress		wifi_ip_t;
typedef uint8_t*		wifi_mac_t;
typedef uint8_t			wifi_status_t;
typedef int32_t 		wifi_rssi_t;

/**
 * @brief wifi attributes
 */
typedef struct {

	// Internals
	char*				fw_ver;
	char*				drv_ver;

	// Basic IP attributes
	IPAddress* 			ip;
	IPAddress* 			dns;
	IPAddress* 			gateway;
	IPAddress* 			subnet;

	// Wifi handle
	WiFiClass*			handle;

}wifi_attributes_t;

/**
 * @brief Wifi Interface Class
 *
 * This is the interface that must be called in order to access
 * the wifi IP stack and to communicate over wifi.
 */
class wifi {

	/*
	 * Private access attributes
	 */
	private:

		/*
		 * Interfaces
		 */
		WiFiClass*			wifi_if;		/**< The wifi interface */
		WifiIPStack*		stack_if;		/**< The IP stack on the wifi stack */

		/*
		 * Attributes
		 */
		wifi_attributes_t*	attr;			/**< The wifi attributes */

		/*
		 * The class status
		 */
		status_code_t 		status;

	/*
	 * Public access methods
	 */
	public:

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
		wifi(WifiIPStack* stack, wifi_attributes_t* settings);

		/**
		 * @brief The Default Deconstructor for the class.
		 */
		~wifi(){};


		/**
		 * @brief The Connect method for connecting to the wifi AP
		 *
		 * This method connects this node to a wifi AP based on the
		 * SSID past and th epast password. We should return true or false
		 * base don th esuccess of the connection.
		 *
		 * @param ssid		The SSID of the wifi AP
		 * @param pass		The password of the wifi AP
		 * @return status	The status
		 */
		wl_status_t connect(wifi_ssid_t ssid, wifi_pass_t pass);

		/**
		 * @brief The Disconnection mechansim for the wifi conenction.
		 *
		 * This method disconnects the node from the AP and returns
		 * a bool representing the success of the disconnection.
		 *
		 * @return status	The status
		 */
		wl_status_t disconnect();

	    /**
	     * @brief Start scan WiFi networks available
	     *
	     * @return int8_t	Number of discovered networks
	     */
	    int8_t scan_networks();

	    /**
	     * @brief Gets the internal wifi interface class.
	     *
	     * @return handle	The wifi class handle
	     */
	    WiFiClass* get_if();

	    /**
	     * @brief Gets the wifi internal stack handle.
	     *
	     * @return handle	The wifi handler stack handle.
	     */
	    WifiIPStack* get_stack();

	    /**
	     * @brief Gets the internal status of the driver.
	     *
	     * @return status	The status
	     */
	    status_code_t get_status(){
	    	return status;
	    }

};

/**< Typedef wifi */
typedef wifi wifi_t;

#endif /* SERVICES_COMMS_WIFI_H_ */
