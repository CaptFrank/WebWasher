#include "Energia.h"

void setup();
void loop();

#line 1 "node.ino"







#include <platform/platform.h>




#include <service/services.h>




#include <task/tasks.h>




bus_i2c_t* bus 		= new bus_i2c_t();




tmp006_t* tmp006 	= new tmp006_t(bus);
bma222_t* bma222 	= new bma222_t(bus);




sensor_t* sensors[NUMBER_OF_SENSORS + 1]	= {
		(sensor_t*)tmp006,
		(sensor_t*)bma222,
		NULL
};





static formatter_t* format = new formatter_t();




static coms_t* coms = new coms_t();




static task_t* tasks[TASK_NUMBER] 	= {

#ifdef DAQ_TASK_ENABLE
		







		new daq_t		(sensors ),
#endif

#ifdef UPDATE_TASK_ENABLE
		







		new update_t	(),
#endif

#ifdef PUBLISH_TASK_ENABLE
		







		new publish_t	(),
#endif

#ifdef IDLE_TASK_ENABLE
		








		new idle_t		(),
#endif
};

void setup() {

	












	


	system_t::BIOS_setup(sensors);
	coms->connect(INTERFACE_BOTH);

	


	system_t::BIOS_register((sensor_t*)tmp006);
	system_t::BIOS_register((sensor_t*)bma222);

	


	system_t::BIOS_boot(tasks);
}

void loop() {

	












	


	system_t::BIOS_run();
}


