#include "MQTTRollerShutter.h"

MQTTRollerShutter::MQTTRollerShutter(const RollerShutter& rollerShutter, const MQTTClient& mqttClient, const String& channelName) {  
  this->mqttChannelName = channelName;
  this->rollerShutter = &rollerShutter;
  this->mqttClient = &mqttClient;
}

void MQTTRollerShutter::setup() {
  this->mqttClient->addMessageListener(this->mqttChannelName, *this);  
}

void MQTTRollerShutter::onMessageReceived(const String& message) {
  if (message == MQTT_UP_COMMAND){
    this->rollerShutter->moveUp();
  } else if (message == MQTT_DOWN_COMMAND) {
    this->rollerShutter->moveDown();
  } else if (message == MQTT_STOP_COMMAND) {
    this->rollerShutter->halt();
  }
}

void MQTTRollerShutter::onMQTTMessageReceived(const String& channelName, const String& message) { 
  if (this->mqttChannelName == channelName) {    
    this->onMessageReceived(message);
  }
}

