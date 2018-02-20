#include <Arduino.h>
#include "ISetupable.h"
#include "ILoopable.h"
#include "IRollerShutter.h"
#include "IRollerShutterStateListener.h"
#include <LinkedList.h>


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
#define ROLLERSHUTTER_MIN_POSITION_CHANGE 250 // minimum required move duration when setting position (millis)

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
    long stopPosition;
    bool positionUnknown;

    LinkedList<IRollerShutterStateListener*> *stateListeners;

    void onStateChanged();
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

    void setPosition(double);

    bool addStateListener(IRollerShutterStateListener*);
};

#endif
