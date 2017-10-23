#include <Arduino.h>
#include "RollerShutter.h"
#include "IMQTTMessageListener.h"
#include "MQTTClient.h"
#ifndef MQTTROLLERSHUTTER_H
#define MQTTROLLERSHUTTER_H

#define MQTT_UP_COMMAND "OPEN"
#define MQTT_DOWN_COMMAND "CLOSE"
#define MQTT_STOP_COMMAND "STOP"

class MQTTRollerShutter : IMQTTMessageListener {
  private:         
    const MQTTClient* mqttClient;
    const RollerShutter* rollerShutter;
    const String mqttChannelName;

    void onMessageReceived(const String&);
  public:  
    MQTTRollerShutter(const RollerShutter&, const MQTTClient&, const String&);
    void setup();    
    
    /*IMQTTMessageListener Implementation */
    void onMQTTMessageReceived(const String&, const String&);
};

#endif
