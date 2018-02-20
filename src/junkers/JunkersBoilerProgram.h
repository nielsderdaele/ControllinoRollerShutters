#include "IProgram.h"
#include <Controllino.h>
#ifndef JUNKERSBOILERPROGRAM_H
#define JUNKERSBOILERPROGRAM_H

class JunkersBoilerProgram : public IProgram {
  private:         
    unsigned long lastReadMillis;
    byte inputPin;

  public:  
    JunkersBoilerProgram(byte inputPin);
    
    void setup();
    void loop();
};

#endif
