#include "RollerShuttersProgram.h"
#include "RollerShutterGroup.h"
#include "RollerShutter.h"
#include "ButtonRollerShutter.h"
#include "MQTTRollerShutter.h"
#include <Controllino.h>

RollerShutterProgram::RollerShutterProgram(const MQTTClient& mqttClient) {
  this->mqttClient = &mqttClient;
  this->rollerShutters = new LinkedList<IRollerShutter*>();
  this->loopables = new LinkedList<ILoopable*>();
}

void RollerShutterProgram::setupRollerShutterGroup(const RollerShutterGroupConfig& config) {
  Button* btnUp = new Button(config.buttonUp);
  Button* btnDown = new Button(config.buttonDown);

  LinkedList<IRollerShutter*> *groupRollerShutters = new LinkedList<IRollerShutter*>();
  for (int i = 0; i < config.numberOfRollerShutters; i++) {
    byte id = config.rollerShutters[i];

    for (int j = 0; j < this->rollerShutters->size(); j++) {
      if (this->rollerShutters->get(j)->getId() == id) {
        groupRollerShutters->add(rollerShutters->get(j));
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
  this->loopables->add(btnUp);
  this->loopables->add(btnDown); 
}

void RollerShutterProgram::setupRollerShutter(const RollerShutterConfig& config) {
  //Serial.println("Roller shutter");
  //Serial.println(config.buttonUp);
  //Serial.println(config.buttonDown);
  //Serial.println(config.relaisLive);
  //Serial.println(config.relaisUpDown);
  //Serial.println(config.upDownDuration);
  
  /* Setup roller shutter */
  RollerShutter* shutter = new RollerShutter(config.id, config.relaisLive, config.relaisUpDown, config.upDownDuration);    
  shutter->setup(); 
  this->loopables->add(shutter);

  /* Setup MQTT shutter*/
  String mqttId = String(config.id);
  String mqttChannel = String("home-assistant/cover/" + mqttId);
  MQTTRollerShutter* mqttShutter = new MQTTRollerShutter(*shutter, *this->mqttClient, mqttChannel);
  mqttShutter->setup();  
  

  /* Setup button shutter */
  if (config.hasButton == 1) {    
    Button* btnUp = new Button(config.buttonUp);
    Button* btnDown = new Button(config.buttonDown);
    ButtonRollerShutter* buttonRollerShutter = new ButtonRollerShutter(shutter, btnUp, btnDown, config.buttonClickEnabled);

    btnUp->setup();
    btnDown->setup(); 
    buttonRollerShutter->setup();

    this->loopables->add(btnUp);
    this->loopables->add(btnDown);
  }  

  /* Add to rollershutters */
  this->rollerShutters->add(shutter);
}

void RollerShutterProgram::setup() {
  /* Retrieve roller shutter configuration */
  Configuration* configuration = Configuration::load();

  /* Configure each roller shutter */
  for (int i = 0; i < configuration->getNumberOfRollerShutters(); i++) {
    RollerShutterConfig rollerShutterConfig = configuration->getRollerShutters()[i];
    if (rollerShutterConfig.enabled) {
      this->setupRollerShutter(rollerShutterConfig);
    }
  }  

  /* Configure each roller shutter group */
  for (int i = 0; i < configuration->getNumberOfRollerShutterGroups(); i++) {
    RollerShutterGroupConfig groupConfig = configuration->getRollerShutterGroups()[i];
    if (groupConfig.enabled) {
      this->setupRollerShutterGroup(groupConfig);
    }
  } 
}


void RollerShutterProgram::loop() {   
  for(int i = 0; i < this->loopables->size(); i++){
		ILoopable* loopable = this->loopables->get(i);
    loopable->loop();
  }
  delay(5); /* wait 5 ms */
}




