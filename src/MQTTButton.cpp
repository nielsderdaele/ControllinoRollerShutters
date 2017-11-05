#include "MQTTButton.h"


MQTTButton::MQTTButton(const Button& button, const MQTTClient& mqttClient, const String& channelName) {
  this->button = &button;
  this->mqttClient = &mqttClient;
  this->mqttChannelName = channelName;
}

void MQTTButton::setup(){
  this->button->addButtonListener(this);
}


/* IButtonListener implementation */
void MQTTButton::onButtonClicked(void* sender, int numberOfClicks){  
  this->mqttClient->publishMessage(this->mqttChannelName, String(numberOfClicks));
}
void MQTTButton::onButtonHold(void* sender) {
  this->mqttClient->publishMessage(this->mqttChannelName, String(-1));
}
void MQTTButton::onButtonRelease(void* sender){
  this->mqttClient->publishMessage(this->mqttChannelName, String(0));
}