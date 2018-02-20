#include <Arduino.h>
#include "Button.h"
#include "IButtonListener.h"
#include "..\mqtt\MQTTClient.h"
#ifndef MQTTBUTTON_H
#define MQTTBUTTON_H

#define MQTT_UP_COMMAND "OPEN"
#define MQTT_DOWN_COMMAND "CLOSE"
#define MQTT_STOP_COMMAND "STOP"

class MQTTButton : IButtonListener {
  private:         
    const MQTTClient* mqttClient;
    const String mqttChannelName;
    const Button* button;

  public:  
    MQTTButton(const Button&, const MQTTClient&, const String&);
    void setup();    
    

    /* IRollerShutterStateListener implementation */
    void onButtonClicked(void*, int);    
    virtual void onButtonHold(void*) = 0;   
    virtual void onButtonRelease(void*) = 0; 
};

#endif
