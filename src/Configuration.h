#include <Arduino.h>
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIGURATION_VERSION 1
#define NUMBER_OF_EEPROM_ROLLERSHUTTER_CONFIGURATIONS 15

struct RollerShutterConfig {
  byte id;
  byte enabled;  
  byte relaisUpDown;
  byte relaisLive;
  byte hasButton;
  byte buttonUp;
  byte buttonDown;
  byte buttonClickEnabled;  
  int upDownDuration;    
};

struct RollerShutterGroupConfig {
  byte enabled;
  byte buttonUp;
  byte buttonDown;
  byte* rollerShutters;
  byte numberOfRollerShutters;
};


class Configuration {
  private:
    int numberOfRollerShutters;
    int numberOfRollerShutterGroups;
    RollerShutterConfig* rollerShutters;
    RollerShutterGroupConfig* rollerShutterGroups;
  
  public:  
    static Configuration* loadFromEEPROM();  
    static Configuration* load();

    int getNumberOfRollerShutters();
    int getNumberOfRollerShutterGroups();
    RollerShutterConfig* getRollerShutters();
    RollerShutterGroupConfig* getRollerShutterGroups();

    void saveToEEPROM();
    
    Configuration(int, RollerShutterConfig*, int, RollerShutterGroupConfig*);   
};

#endif

