#include <Arduino.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include "IMQTTMessageListener.h"
#include "Client.h"
#include "ISetupable.h"
#include "ILoopable.h"
#include <LinkedList.h>

#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

class MQTTClient : public ISetupable, public ILoopable {
  private:
    unsigned long connectionAttemptMillis;         
    Client *networkClient;
    PubSubClient *client; 
    LinkedList<String> *channels;
    LinkedList<IMQTTMessageListener*> *listeners;

    void onMessageReceived(const String& channel, const String& message);
  public:  
    MQTTClient(const IPAddress&, int, const Client&);
    void publishMessage(const String& channelName, const String& message);
    void publishMessage(const String& channelName, const String& message, bool retained);
    void addMessageListener(const String&, const IMQTTMessageListener&);
    
    void setup();    
    void loop();

  friend void MQTTClient_MessageReceived(MQTTClient* mqttClient, const String& channel, const String& message);
};

#endif
