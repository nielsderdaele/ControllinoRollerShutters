#include "Configuration.h"
#include <EEPROM.h>
#include <Controllino.h>

/* Pin configuration */
#define BTN_0_UP CONTROLLINO_A0
#define BTN_0_DOWN CONTROLLINO_A1
#define ROLLER_0_LIVE CONTROLLINO_R4
#define ROLLER_0_UPDOWN CONTROLLINO_R5

#define BTN_1_UP CONTROLLINO_A2
#define BTN_1_DOWN CONTROLLINO_A3
#define ROLLER_1_LIVE CONTROLLINO_R2
#define ROLLER_1_UPDOWN CONTROLLINO_R3

#define BTN_2_UP CONTROLLINO_A4
#define BTN_2_DOWN CONTROLLINO_A5
#define ROLLER_2_LIVE CONTROLLINO_R0
#define ROLLER_2_UPDOWN CONTROLLINO_R1


#define BTN_3_UP CONTROLLINO_A6
#define BTN_3_DOWN CONTROLLINO_A7
#define ROLLER_3_LIVE CONTROLLINO_R6
#define ROLLER_3_UPDOWN CONTROLLINO_R9


#define BTN_4_UP CONTROLLINO_A8
#define BTN_4_DOWN CONTROLLINO_A9
#define ROLLER_4_LIVE CONTROLLINO_R10
#define ROLLER_4_UPDOWN CONTROLLINO_R13


#define BTN_GROUP_0_UP CONTROLLINO_A10
#define BTN_GROUP_0_DOWN CONTROLLINO_A11

Configuration* Configuration::load() {
  int numberOfConfigurations = 6;

  /* Create array with roller shutter configurations */
  RollerShutterConfig* rollerShutterConfigs = new RollerShutterConfig[numberOfConfigurations];

  /* Living voorzijde rechts (tuin) */
  rollerShutterConfigs[0].id = 0;
  rollerShutterConfigs[0].enabled = 1;  
  rollerShutterConfigs[0].relaisUpDown = ROLLER_0_UPDOWN;
  rollerShutterConfigs[0].relaisLive = ROLLER_0_LIVE;
  rollerShutterConfigs[0].buttonDown = BTN_0_DOWN;
  rollerShutterConfigs[0].buttonUp = BTN_0_UP;
  rollerShutterConfigs[0].buttonClickEnabled = 1;
  rollerShutterConfigs[0].upDownDuration = 20000;

  /* Living voorzijde links (oprit) */
  rollerShutterConfigs[1].id = 1;
  rollerShutterConfigs[1].enabled = 1;
  rollerShutterConfigs[1].relaisUpDown = ROLLER_1_UPDOWN;
  rollerShutterConfigs[1].relaisLive = ROLLER_1_LIVE;
  rollerShutterConfigs[1].buttonDown = BTN_1_DOWN;
  rollerShutterConfigs[1].buttonUp = BTN_1_UP;
  rollerShutterConfigs[1].buttonClickEnabled = 1;
  rollerShutterConfigs[1].upDownDuration = 20000;

  /* Living oprit */
  rollerShutterConfigs[2].id = 2;
  rollerShutterConfigs[2].enabled = 1;
  rollerShutterConfigs[2].relaisUpDown = ROLLER_2_UPDOWN;
  rollerShutterConfigs[2].relaisLive = ROLLER_2_LIVE;
  rollerShutterConfigs[2].buttonDown = BTN_2_DOWN;
  rollerShutterConfigs[2].buttonUp = BTN_2_UP;
  rollerShutterConfigs[2].buttonClickEnabled = 1;
  rollerShutterConfigs[2].upDownDuration = 20000;

  /* Living tuin */  
  rollerShutterConfigs[3].id = 3;
  rollerShutterConfigs[3].enabled = 1;
  rollerShutterConfigs[3].relaisUpDown = ROLLER_3_UPDOWN;
  rollerShutterConfigs[3].relaisLive = ROLLER_3_LIVE;
  rollerShutterConfigs[3].buttonDown = BTN_3_DOWN;
  rollerShutterConfigs[3].buttonUp = BTN_3_UP;
  rollerShutterConfigs[3].buttonClickEnabled = 1;
  rollerShutterConfigs[3].upDownDuration = 20000;
  /* Keuken */
  rollerShutterConfigs[4].id = 4;
  rollerShutterConfigs[4].enabled = 1;
  rollerShutterConfigs[4].relaisUpDown = ROLLER_4_UPDOWN;
  rollerShutterConfigs[4].relaisLive = ROLLER_4_LIVE;
  rollerShutterConfigs[4].buttonDown = BTN_4_DOWN;
  rollerShutterConfigs[4].buttonUp = BTN_4_UP;
  rollerShutterConfigs[4].buttonClickEnabled = 1;
  rollerShutterConfigs[4].upDownDuration = 15000;

  /* Waskot */
  rollerShutterConfigs[5].id = 5;
  rollerShutterConfigs[5].enabled = 0;

  /* */
  int numberOfGroupConfigurations = 1;
  
  /* Create array with roller shutter configurations */
  RollerShutterGroupConfig* rollerShutterGroupConfigs = new RollerShutterGroupConfig[numberOfGroupConfigurations];

  byte* rollerShuttersGroup0 = new byte[5];
  rollerShuttersGroup0[0] = 0;
  rollerShuttersGroup0[1] = 1;
  rollerShuttersGroup0[2] = 2;
  rollerShuttersGroup0[3] = 3;
  rollerShuttersGroup0[4] = 4;

  rollerShutterGroupConfigs[0].enabled = 1;
  rollerShutterGroupConfigs[0].buttonUp = BTN_GROUP_0_UP;
  rollerShutterGroupConfigs[0].buttonDown = BTN_GROUP_0_DOWN;
  rollerShutterGroupConfigs[0].rollerShutters = rollerShuttersGroup0;
  rollerShutterGroupConfigs[0].numberOfRollerShutters = 5;


  Configuration* conf = new Configuration(numberOfConfigurations, rollerShutterConfigs, numberOfGroupConfigurations, rollerShutterGroupConfigs);
  return conf;
}

Configuration* Configuration::loadFromEEPROM() {
  /*******************************
     version (1 byte)
     number of configs (1 byte)
     --
     configs
     ..
     --
   ********************************/
  
  Serial.println("Reading configuration from EEPROM");
  Serial.flush();
  
  int address = 0;
  byte version = 0;
  boolean isModified = false;

  /* Create array with roller shutter configurations */
  RollerShutterConfig* rollerShutterConfigs = new RollerShutterConfig[NUMBER_OF_EEPROM_ROLLERSHUTTER_CONFIGURATIONS];

  /* Configuration object */
  Configuration* conf = new Configuration(NUMBER_OF_EEPROM_ROLLERSHUTTER_CONFIGURATIONS, rollerShutterConfigs, 0, 0);

  /* Read version from EEPROM */
  version = EEPROM.read(address);
  Serial.print("  Version: ");
  Serial.println(version);
  Serial.flush();
  
  
  if (version == CONFIGURATION_VERSION) {   
    
    /*Move address to the next byte after version.*/
    address = sizeof(byte);

    /* Read number of configurations stored in EEPROM */
    byte numberOfConfigurations = EEPROM.read(address);    

    Serial.print("  Number of configurations: ");
    Serial.println(numberOfConfigurations);
    Serial.flush();

    /* Read all configurations from EEPROM*/
    if (numberOfConfigurations >= 0 && numberOfConfigurations <= NUMBER_OF_EEPROM_ROLLERSHUTTER_CONFIGURATIONS) {
      address += sizeof(byte);

      int rollerShutterConfigSize = sizeof(RollerShutterConfig);      
      for (int i = 0; i < numberOfConfigurations; i++) {
        EEPROM.get(address, rollerShutterConfigs[i]);

        Serial.print("  Configuration ");
        Serial.println(i);
        Serial.print("    id: ");
        Serial.println(rollerShutterConfigs[i].id);
        Serial.print("    enabled: ");
        Serial.println(rollerShutterConfigs[i].enabled);
        Serial.print("    relaisUpDown: ");
        Serial.println(rollerShutterConfigs[i].relaisUpDown);
        Serial.print("    relaisLive: ");
        Serial.println(rollerShutterConfigs[i].relaisLive);
        Serial.print("    buttonUp: ");        
        Serial.println(rollerShutterConfigs[i].buttonUp);
        Serial.print("    buttonDown: ");
        Serial.println(rollerShutterConfigs[i].buttonDown);
        Serial.print("    upDownDuration: ");
        Serial.println(rollerShutterConfigs[i].upDownDuration);
        Serial.flush();
        address += rollerShutterConfigSize;
      }
    }

    isModified = (numberOfConfigurations != NUMBER_OF_EEPROM_ROLLERSHUTTER_CONFIGURATIONS);
  } else {
    isModified = true;
  }

  

  /* Save to EEPROM 
  if (isModified) { conf->saveToEEPROM();}
  */

  return conf;
}

Configuration::Configuration(int numberOfConfigurations, RollerShutterConfig* rollerShutterConfigs, int numberOfGroupConfigurations, RollerShutterGroupConfig* rollerShutterGroupConfigs) {
  this->rollerShutters = rollerShutterConfigs;
  this->rollerShutterGroups = rollerShutterGroupConfigs;
  this->numberOfRollerShutters = numberOfConfigurations;
  this->numberOfRollerShutterGroups = numberOfGroupConfigurations;
}

int Configuration::getNumberOfRollerShutters() {
  return this->numberOfRollerShutters;
}

int Configuration::getNumberOfRollerShutterGroups() {
  return this->numberOfRollerShutterGroups;
}

RollerShutterConfig* Configuration::getRollerShutters() {
  return this->rollerShutters;
}

RollerShutterGroupConfig* Configuration::getRollerShutterGroups() {
  return this->rollerShutterGroups;
}

void Configuration::saveToEEPROM() {
  Serial.print("Writing configuration to EEPROM");
  Serial.flush();
    
  int address = 0;
  EEPROM.write(address, CONFIGURATION_VERSION);

  address = sizeof(byte);
  EEPROM.write(address, this->numberOfRollerShutters);

  address += sizeof(byte);
  int rollerShutterConfigSize = sizeof(RollerShutterConfig);      
  for (int i = 0; i < this->numberOfRollerShutters; i++) {
    EEPROM.put(address, this->rollerShutters[i]);    
    address += rollerShutterConfigSize;
  }
}

