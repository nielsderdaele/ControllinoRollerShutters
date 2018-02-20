#include "IProgram.h"
#include <LinkedList.h>
#include "IRollerShutter.h"
#include "..\mqtt\MQTTClient.h"
#include "Configuration.h"
#include "Button.h"

#ifndef ROLLERSHUTTERPROGRAM_H
#define ROLLERSHUTTERPROGRAM_H

class RollerShutterProgram : public IProgram {
    private: 
      MQTTClient* mqttClient;
      LinkedList<IRollerShutter*> *rollerShutters;    
      LinkedList<ILoopable*> *loopables;

      void setupRollerShutterGroup(const RollerShutterGroupConfig& config);
      void setupRollerShutter(const RollerShutterConfig&);

    public:  
      RollerShutterProgram(const MQTTClient&);    
  
      void setup();
      void loop();
};
#endif