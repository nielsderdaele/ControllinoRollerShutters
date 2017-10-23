#include <Arduino.h>
#include "ISetupable.h"
#include "ILoopable.h"
#include "IRollerShutter.h"
#ifndef ROLLERSHUTTER_H
#define ROLLERSHUTTER_H

#define ROLLERSHUTTER_RELAY_DELAY 25
#define ROLLERSHUTTER_UP HIGH
#define ROLLERSHUTTER_DOWN LOW
#define ROLLERSHUTTER_ON HIGH
#define ROLLERSHUTTER_OFF LOW
#define ROLLERSHUTTER_STATE_UNDETERMINED -1
#define ROLLERSHUTTER_STATE_CLOSED 0
#define ROLLERSHUTTER_STATE_OPEN 1
#define ROLLERSHUTTER_MAX_MOVEDURATION 240000 // 4 minutes

class RollerShutter : public IRollerShutter, public ISetupable, public ILoopable {
  private:
    byte id; 
    byte livePin;  
    byte upDownPin;
    boolean movingDown;
    boolean movingUp;
    int state;
    int moveDuration;
    unsigned long moveDownMillis;
    unsigned long moveUpMillis;  
    long currentPosition;  
    byte positionUnknown;
  public:  
    RollerShutter(byte, byte, byte, int);
    void setup();
    void loop();

    byte getId();

    boolean isMovingDown();
    boolean isMovingUp();    
    
    void moveDown();
    void moveUp();
    void halt();
};

#endif
