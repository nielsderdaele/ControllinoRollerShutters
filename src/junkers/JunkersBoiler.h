#include "ILoopable.h"
#include "ISetupable.h"
#include <Controllino.h>
#include "../mqtt/MQTTClient.h"
#ifndef JUNKERSBOILER_H
#define JUNKERSBOILER_H

#define JUNKERSBOILER_MAX_VALUE 879
#define JUNKERSBOILER_MIN_VALUE 184

class JunkersBoiler : public ILoopable, public ISetupable {
  private:    
    const MQTTClient* mqttClient;
    const String mqttChannel;
    unsigned long lastReadMillis;
    byte inputPin;
    int currentValue;

    double getCurrentThermostatVoltage();

  public:  
    JunkersBoiler(byte inputPin, const MQTTClient&);
    
    void setup();
    void loop();
};

#endif