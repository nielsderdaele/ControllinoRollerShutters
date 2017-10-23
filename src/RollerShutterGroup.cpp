#include "RollerShutterGroup.h"

RollerShutterGroup::RollerShutterGroup(LinkedList<IRollerShutter*> *rollerShutters) {
    this->rollerShutters = rollerShutters;

    Serial.println("Number of rollershutters");
    Serial.println(this->rollerShutters->size());

}

byte RollerShutterGroup::getId(){
    return 0;
}

void RollerShutterGroup::moveDown(){
    for (int i = 0; i < this->rollerShutters->size() ; i++) {
        IRollerShutter *rollerShutter = this->rollerShutters->get(i);
        rollerShutter->moveDown();        
        delay(ROLLERSHUTTER_GROUP_DELAY);
    }
}

void RollerShutterGroup::moveUp(){
    for (int i = 0; i < this->rollerShutters->size() ; i++) {
        IRollerShutter *rollerShutter = this->rollerShutters->get(i);        
        rollerShutter->moveUp();
        delay(ROLLERSHUTTER_GROUP_DELAY);
    }
}
  
void RollerShutterGroup::halt(){
    for (int i = 0; i < this->rollerShutters->size() ; i++) {
        IRollerShutter *rollerShutter = this->rollerShutters->get(i);        
        rollerShutter->halt();
        delay(ROLLERSHUTTER_GROUP_DELAY);
    }
}

boolean RollerShutterGroup::isMovingDown(){
    boolean isMovingDown = false;
    for (int i = 0; i < this->rollerShutters->size() ; i++) {
        IRollerShutter *rollerShutter = this->rollerShutters->get(i);
        if (rollerShutter->isMovingDown()) {
            isMovingDown = true;
        }
    }
    return isMovingDown;
}

boolean RollerShutterGroup::isMovingUp(){
    boolean isMovingUp = false;
    for (int i = 0; i < this->rollerShutters->size() ; i++) {
        IRollerShutter *rollerShutter = this->rollerShutters->get(i);
        if (rollerShutter->isMovingUp()) {
            isMovingUp = true;
        }
    }
    return isMovingUp;  
}
  