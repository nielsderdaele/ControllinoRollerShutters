#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <stdint.h>
#include "Button.h"
#include "ButtonRollerShutter.h"
#include "MQTTRollerShutter.h"
#include "MQTTClient.h"
#include "RollerShutter.h"
#include "RollerShutterGroup.h"
#include "Configuration.h"

LinkedList<MQTTClient*> mqttClient_callbackClients = LinkedList<MQTTClient*>();

/* MAC address (ethernet) */
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

/* Local address */
IPAddress ip(192, 168, 1, 10);
IPAddress myDns(192,168,1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

/* Network client */
Client* client = new EthernetClient();

/* Remote address */
IPAddress serverAddr(192,168,1,100); // ip address to ping

/* MQTT client => connects to home assistant */
MQTTClient mqttClient(serverAddr, 1883, *client);

/* List of loopables */
LinkedList<ILoopable*> loopables = LinkedList<ILoopable*>();

/* List of roller shutters */
LinkedList<IRollerShutter*> rollerShutters = LinkedList<IRollerShutter*>();

unsigned long mqttConnectionAttemptMillis = 0;

//EthernetClient ethClient;
//aREST rest = aREST();
//EthernetServer server(80);

void setupRollerShutterGroup(const RollerShutterGroupConfig& config) {
  Button* btnUp = new Button(config.buttonUp);
  Button* btnDown = new Button(config.buttonDown);

  LinkedList<IRollerShutter*> *groupRollerShutters = new LinkedList<IRollerShutter*>();
  for (int i = 0; i < config.numberOfRollerShutters; i++) {
    byte id = config.rollerShutters[i];

    for (int j = 0; j < rollerShutters.size(); j++) {
      if (rollerShutters.get(j)->getId() == id) {
        groupRollerShutters->add(rollerShutters.get(j));
      }
    }
  }

  RollerShutterGroup* shutterGroup = new RollerShutterGroup(groupRollerShutters); 
  ButtonRollerShutter* btnRollerShutter = new ButtonRollerShutter(shutterGroup, btnUp, btnDown, 1);

  /* Setup hardware */
  btnUp->setup();
  btnDown->setup();
  btnRollerShutter->setup();

  /* Add to loopables */
  loopables.add(btnUp);
  loopables.add(btnDown); 
}

void setupRollerShutter(const RollerShutterConfig& config) {
  Serial.println("Roller shutter");
  Serial.println(config.buttonUp);
  Serial.println(config.buttonDown);
  Serial.println(config.relaisLive);
  Serial.println(config.relaisUpDown);
  Serial.println(config.upDownDuration);
  
  /* Roller Shutter */
  Button* btnUp = new Button(config.buttonUp);
  Button* btnDown = new Button(config.buttonDown);
  RollerShutter* shutter = new RollerShutter(config.id, config.relaisLive, config.relaisUpDown, config.upDownDuration);  
  ButtonRollerShutter* buttonRollerShutter = new ButtonRollerShutter(shutter, btnUp, btnDown, config.buttonClickEnabled);
  String mqttId = String(config.id);
  String mqttChannel = String("home-assistant/cover/" + mqttId);
  MQTTRollerShutter* mqttShutter = new MQTTRollerShutter(*shutter, mqttClient, mqttChannel);

  /* Setup hardware */
  btnUp->setup();
  btnDown->setup();
  shutter->setup();  
  buttonRollerShutter->setup();
  mqttShutter->setup();  

  /* Add to loopables */
  loopables.add(btnUp);
  loopables.add(btnDown);
  loopables.add(shutter);

  /* Add to rollershutters */
  rollerShutters.add(shutter);
}

void setup() {
  /* Initialize Serial Port (debugging) */
  Serial.begin(9600);
  
  /* Retrieve roller shutter configuration */
  Configuration* configuration = Configuration::load();

  /* Add MQTT client to loopables */
  loopables.add(&mqttClient);

  /* Configure each roller shutter */
  for (int i = 0; i < configuration->getNumberOfRollerShutters(); i++) {
    RollerShutterConfig rollerShutterConfig = configuration->getRollerShutters()[i];
    if (rollerShutterConfig.enabled) {
      setupRollerShutter(rollerShutterConfig);
    }
  }  

  /* Configure each roller shutter group */
  for (int i = 0; i < configuration->getNumberOfRollerShutterGroups(); i++) {
    RollerShutterGroupConfig groupConfig = configuration->getRollerShutterGroups()[i];
    if (groupConfig.enabled) {
      setupRollerShutterGroup(groupConfig);
    }
  } 

  /* Initialize Ethernet Shield */
  Ethernet.begin(mac, ip, myDns, gateway, subnet);  

  Serial.println(Ethernet.localIP());

  /* Start listening for REST clients */
  //server.begin();
}

//void reconnect() {
//  Serial.print("Attempting MQTT connection...");
//  Serial.flush();
    
  /* Try to connect */
//  boolean connected = client.connect("ControllinoMEGA", "pi", "raspberry");
//  if (connected){
//    client.subscribe("home-assistant/cover/living/oprit/set"); /* Subscribe to living roller shutter*/
//    Serial.println(" Connected");
//  } else {
//    Serial.println(" FAILED");
//  }
//  mqttConnectionAttemptMillis = millis();
//  Serial.flush();  
//}


void loop() {   
//  if (!client.connected()) {
//    if (millis() - mqttConnectionAttemptMillis > 5000) {
//      reconnect();        
//   }
//  } else {
//    client.loop();
//  }
  //client.publish("SomeTopic", "Hello", true);
  for(int i = 0; i < loopables.size(); i++){
		ILoopable* loopable = loopables.get(i);
    loopable->loop();
  }
  /* listen for incoming clients */
  //EthernetClient client = server.available();
  //rest.handle(client);
  
  delay(5); /* wait 5 ms */
}




