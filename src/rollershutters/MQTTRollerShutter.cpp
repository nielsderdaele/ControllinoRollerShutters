#include "MQTTRollerShutter.h"

MQTTRollerShutter::MQTTRollerShutter(const RollerShutter& rollerShutter, const MQTTClient& mqttClient, const String& channelName) {  
  String temp = (channelName + "/set");
  this->mqttChannelName = temp;
  temp = (channelName + "/state");
  this->mqttStateChannelName = temp;
  temp = (channelName + "/position");
  this->mqttPositionChannelName = temp;
  this->rollerShutter = &rollerShutter;
  this->mqttClient = &mqttClient;
}

void MQTTRollerShutter::setup() {
  this->mqttClient->addMessageListener(this->mqttChannelName, *this);  
  this->mqttClient->addMessageListener(this->mqttPositionChannelName, *this);
  this->rollerShutter->addStateListener(this);
}

void MQTTRollerShutter::onCommandMessageReceived(const String& message) {
  if (message == MQTT_UP_COMMAND){
    this->rollerShutter->moveUp();
  } else if (message == MQTT_DOWN_COMMAND) {
    this->rollerShutter->moveDown();
  } else if (message == MQTT_STOP_COMMAND) {
    this->rollerShutter->halt();
  }
}

void MQTTRollerShutter::onPositionMessageReceived(const String& message) {
  Serial.println("Position message received: " + message);
  long position = message.toInt();
  if (position >= 0 && position <= 100) {
    Serial.println("Valid position");
    this->rollerShutter->setPosition((double)position/100.0);
  }
}

void MQTTRollerShutter::onMQTTMessageReceived(const String& channelName, const String& message) { 
  if (this->mqttChannelName == channelName) {    
    this->onCommandMessageReceived(message);
  } else if (this->mqttPositionChannelName == channelName) {
    this->onPositionMessageReceived(message);
  }
}

void MQTTRollerShutter::onStateChanged(void* sender, double currentPosition){
  Serial.println("State Changed: " + String(currentPosition));
  if (currentPosition == 0) {
    this->mqttClient->publishMessage(this->mqttStateChannelName, "0", true);
  } else if (currentPosition >= 1) {
    this->mqttClient->publishMessage(this->mqttStateChannelName, "100", true);
  } else {
    int percentage = (currentPosition * 100);
    this->mqttClient->publishMessage(this->mqttStateChannelName, String(percentage), true);
  }
}