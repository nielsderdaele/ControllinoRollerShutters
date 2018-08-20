#include "MQTTClient.h"

extern LinkedList<MQTTClient*> mqttClient_callbackClients;

void MQTTClient_MessageReceived(MQTTClient* client, const String& channel, const String& message) {
  client->onMessageReceived(channel, message);
}

void MQTTClient_callback(char* topic, byte* payload, unsigned int length) {  
  if (length > 0) {
    /* Convert byte array to String */
    char payloadText[length + 1];
    for (int i = 0; i < length; i++) {
      payloadText[i] = (char)payload[i];
    }
    payloadText[length] = 0; /* 0 terminate string */  

    String channel(topic);
    String message(payloadText);

    Serial.println("MQTT message received. Channel: " + channel + ", Message: " + message);
    for (int i = 0; i < mqttClient_callbackClients.size(); i++) {
      MQTTClient_MessageReceived(mqttClient_callbackClients.get(i), channel, message);
    }
  }
}


MQTTClient::MQTTClient(const IPAddress& server, int port, const Client& client) {
  this->listeners = new LinkedList<IMQTTMessageListener*>();
  this->channels = new LinkedList<String>();
  this->networkClient = &client;
  this->connectionAttemptMillis = 0;
  this->client = new PubSubClient(server, port, *this->networkClient);
  this->client->setCallback(MQTTClient_callback);
  mqttClient_callbackClients.add(this);
}

void MQTTClient::setup() {
}
void MQTTClient::loop() {
  /* TODO: reconnect attempt */
  if (!client->connected()) {
    if (millis() - this->connectionAttemptMillis > 5000) {
      /* Reconnect */
      Serial.print("Attempting MQTT connection...");
      Serial.flush();

      boolean connected = this->client->connect("ControllinoMEGA", "pi", "raspberry");      
      if (connected){
        Serial.println(" Connected");

        /* Subscribe to each requested channel */
        for (int i = 0; i < this->channels->size() ; i++) {
          String channel = this->channels->get(i);
          char channelName[channel.length() + 1];
          channel.toCharArray(channelName, channel.length() + 1);
          client->subscribe(channelName);
          Serial.println("Subscribed to " + channel);
          Serial.flush();  
        }        
      } else {
        Serial.println(" FAILED");
      }

      this->connectionAttemptMillis = millis();
      Serial.flush();  
    }
  } else {
    client->loop();
  }  
}
void MQTTClient::addMessageListener(const String& channelName, const IMQTTMessageListener& listener) {
  this->channels->add(channelName);
  this->listeners->add(&listener);
}
void MQTTClient::onMessageReceived(const String& channel, const String& message) {  
  for (int i = 0; i < this->channels->size(); i++) {
    String listenerChannel = this->channels->get(i);
    IMQTTMessageListener* listener = this->listeners->get(i);    
    if (listenerChannel == channel) {
      listener->onMQTTMessageReceived(channel, message);
    }
  }  
}

void MQTTClient::publishMessage(const String& channelName, const String& message){
  this->publishMessage(channelName, message, false);
}
void MQTTClient::publishMessage(const String& channelName, const String& message, bool retained){
  if (this->client->connected()) {
    this->client->publish(channelName.c_str(), message.c_str(), retained);
  }
}


