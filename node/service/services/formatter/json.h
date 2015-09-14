/*
 * json.h
 *
 *  Created on: Aug 5, 2015
 *      Author: fpapinea
 */

#ifndef SERVICE_SERVICES_COMS_JSON_H_
#define SERVICE_SERVICES_COMS_JSON_H_

/*!
 * \brief Accelerometer JSON Structure
 */
#define 	MQTT_ACC_JSON				"{"								\
											"time:%s,"					\
											"acc:{"						\
												"temp:%d, "				\
												"x:%d, "				\
												"y:%d, "				\
												"z:%d "					\
											"},"						\
										"}"								\

/*!
 * \brief Temperature JSON Structure
 */
#define 	MQTT_TEMP_JSON				"{"								\
											"time:%s,"					\
											"temp:{"					\
												"objtemp:%f,"			\
												"dietemp:%f,"			\
												"volt:%d"				\
											"}"							\
										"}"								\

/*!
 * \brief Heartbeat JSON Structure
 */
#define 	MQTT_HEARTBEAT_JSON			"{"								\
											"time:%s,"					\
											"alive:%d"					\
											"state:%d"					\
										"}"								\

/*!
 * \brief Status JSON Structure
 */
#define 	MQTT_STATUS_JSON			"{"								\
											"time:%s,"					\
											"device:{"					\
												"status:%d,"			\
												"alive:%d,"				\
												"runtime:%d,"			\
												"verion:%d,"			\
												"id:%d"					\
											"}, "						\
											"coms:{"					\
												"mqtt:{"				\
													"status:%d,"		\
													"broker:%s,"		\
													"port:%d,"			\
													"ver:%d,"			\
													"id:%d,"			\
													"topics:{"			\
														"pub:{"			\
															"temp:%s,"	\
															"acc:%s,"	\
															"stat:%s"	\
														"},"			\
														"sub:{"			\
															"glob:%s,"	\
															"local:%s,"	\
														"},"			\
													"},"				\
												"},"					\
												"ip:{"					\
													"status:%d,"		\
													"socket:%d,"		\
													"ip:%s,"			\
													"timeout:%d,"		\
													"ssid:%s,"			\
													"gw:%s,"			\
													"dns:%s,"			\
													"subnet:%s,"		\
												"},"					\
											"},"						\
										"}"								\

/*!
 * \brief Time formatting
 */
#define 	TIME_FORMAT					"%d:%d:%d"



#endif /* SERVICE_SERVICES_COMS_JSON_H_ */
