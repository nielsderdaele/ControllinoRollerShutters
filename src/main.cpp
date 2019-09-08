#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <stdint.h>
#include "IProgram.h"
#include "mqtt\MQTTClient.h"
#include "rollershutters\RollerShuttersProgram.h"
#include "junkers\JunkersBoilerProgram.h"

LinkedList<MQTTClient*> mqttClient_callbackClients = LinkedList<MQTTClient*>();

/* MAC address (ethernet) */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* Local address (STATIC) -> DISABLED */
/* IPAddress ip(192, 168, 1, 10);
IPAddress myDns(192,168,1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);*/

/* Network client */
Client* client = new EthernetClient();

/* Remote address (STATIC) -> DISABLED */
// IPAddress serverAddr(192,168,1,100); // ip address to ping
char* serverAddr = "mqtt.int.nielsderdaele.be";

/* MQTT client => connects to home assistant */
MQTTClient mqttClient(serverAddr, 1883, *client);

/* List of programs */
LinkedList<IProgram*> programs = LinkedList<IProgram*>();

unsigned long mqttConnectionAttemptMillis = 0;

void setup() {
  /* Initialize Serial Port (debugging) */
  Serial.begin(9600);
  
  /* Initialize Ethernet Shield */
  // Ethernet.begin(mac, ip, myDns, gateway, subnet); // Static IP  
  Ethernet.begin(mac); // DHCP
  Serial.println(Ethernet.localIP());

  /* Create Roller shutter program */
  IProgram* program = new RollerShutterProgram(mqttClient);
  programs.add(program);  

  /* Create Junkers boiler program */
  program = new JunkersBoilerProgram(mqttClient);
  programs.add(program);

  /* Setup mqtt client */
  mqttClient.setup();

  /* Setup all programs */
  for(int i = 0; i < programs.size(); i++){
		IProgram* program = programs.get(i);
    program->setup();
  }
}

void loop() {   
  /* Maintain ethernet connection */
  Ethernet.maintain();

  /* Loop mqtt client */
  mqttClient.loop(); 

  /* Loop each program */
  for(int i = 0; i < programs.size(); i++){
		IProgram* program = programs.get(i);
    program->loop();
  }

  delay(5); /* wait 5 ms */
}