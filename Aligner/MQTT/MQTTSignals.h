#ifndef A308MQTTSIGNALS_H
#define A308MQTTSIGNALS_H

#define A308MODULE_CLIENT_ID					"A308Module"

// A308 module ****************************

// Input topics
#define MQTT_A308_SET_MODULE_ACTION			"A308Module/SetModuleAction"

#define MQTT_A308_SET_MODULE_ACTION_ID	0

// Output topics
#define MQTT_A308MODULE_ALIVE					"A308Module/Alive"
#define MQTT_A308MODULE_LIVE_DATA				"A308Module/LiveData"
#define MQTT_A308MODULE_ALIVE_MESSAGE			"Alive"

#define BROKER_ADDRESS "localhost"
#define MQTT_PORT 1883


#endif //A308MQTTSIGNALS_H