#include <Controllino.h>
#include <LinkedList.h>
#include "IRollerShutter.h"

#ifndef ROLLERSHUTTERGROUP_H
#define ROLLERSHUTTERGROUP_H

#define ROLLERSHUTTER_GROUP_DELAY 250

class RollerShutterGroup : public IRollerShutter {
    private: 
      LinkedList<IRollerShutter*> *rollerShutters;    

    public:  
      RollerShutterGroup(LinkedList<IRollerShutter*>*);

      byte getId();

      boolean isMovingDown();
      boolean isMovingUp();    
  
      void moveDown();
      void moveUp();
      void halt();
};
#endif