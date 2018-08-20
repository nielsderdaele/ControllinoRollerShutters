#include "IProgram.h"
#include "JunkersBoiler.h"
#include <Controllino.h>
#ifndef JUNKERSBOILERPROGRAM_H
#define JUNKERSBOILERPROGRAM_H

class JunkersBoilerProgram : public IProgram {
  private:         
    JunkersBoiler* junkersBoiler;

  public:  
    JunkersBoilerProgram(const MQTTClient&);
    
    void setup();
    void loop();
};

#endif
