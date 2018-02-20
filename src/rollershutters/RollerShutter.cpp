#include "RollerShutter.h"

RollerShutter::RollerShutter(byte id, byte livePin, byte upDownPin, int moveDuration) {
  this->id = id;
  this->movingDown = false;
  this->movingUp = false;
  this->positionUnknown = true;
  this->state = ROLLERSHUTTER_STATE_UNDETERMINED;
  this->livePin = livePin;
  this->upDownPin = upDownPin;    
  if (moveDuration <= 0) { moveDuration = ROLLERSHUTTER_MAX_MOVEDURATION;}
  this->moveDuration = moveDuration;
  this->stopPosition = -1;
  this->stateListeners = new LinkedList<IRollerShutterStateListener*>();
}

void RollerShutter::setup() {
  pinMode(this->livePin, OUTPUT);
  pinMode(this->upDownPin, OUTPUT);

  digitalWrite(this->livePin, ROLLERSHUTTER_OFF);
  digitalWrite(this->upDownPin, LOW);  
}
void RollerShutter::loop() {
  /* Check if we should stop moving */
  if (this->isMovingUp()) {    
    unsigned long currentMillis = millis();
    unsigned long movedMillis = currentMillis - moveUpMillis;

    /* check auto stop */
    if (this->stopPosition != -1 && !this->positionUnknown) {
      long newPosition = this->currentPosition + movedMillis;
      if (newPosition >= this->stopPosition) {
        this->halt();
      }
    }
    
    /* cut off if duration has expired */
    if (movedMillis >= this->moveDuration) {
      /* moved longer than max move duration */
      this->halt();      
      
      /* if position was unknown => configure it */
      if (this->positionUnknown) {
        this->positionUnknown = false;
        this->currentPosition = this->moveDuration; /* set to max */
        this->onStateChanged();
      }
    }    
  } else if (this->isMovingDown()) {
    unsigned long currentMillis = millis();
    unsigned long movedMillis = currentMillis - moveDownMillis;
    
    /* check auto stop */
    if (this->stopPosition != -1 && !this->positionUnknown) {
      long newPosition = this->currentPosition - movedMillis;
      if (newPosition <= this->stopPosition) {
        this->halt();
      }
    }


    if (movedMillis >= this->moveDuration) {
      /* moved longer than max move duration */
      this->halt();

      Serial.println("Moved down long enough => halted");

      /* if position was unknown => configure it */
      if (this->positionUnknown) {
        Serial.println("Position now known");
        this->positionUnknown = false;
        this->currentPosition = 0; /* set to min */
        this->onStateChanged();
      } else {
        Serial.println("Position already known");
      }
    }    
  }
}

byte RollerShutter::getId() {
  return this->id;
}


void RollerShutter::setPosition(double position) {
  if (!this->positionUnknown) {
    /* stop roller shutter from moving */
    if (this->isMovingDown() || this->isMovingUp()) {
      this->halt();
    }

    /* calculate new position */    
    long stopPosition;    
    if (position <= 0) {
      stopPosition = 0;      
    } else if (position >= 1) {
      stopPosition = this->moveDuration;      
    } else {
      stopPosition = position*this->moveDuration;
    }

    long positionDiff = stopPosition - this->currentPosition;
    if (positionDiff > ROLLERSHUTTER_MIN_POSITION_CHANGE){
      /* move up */
      this->moveUp();
      /* configure auto stop position */
      this->stopPosition = stopPosition; 
    } else if (positionDiff < -ROLLERSHUTTER_MIN_POSITION_CHANGE) {
      /* move down */
      this->moveDown();
      /* configure auto stop position */
      this->stopPosition = stopPosition; 
    }

  }
}
void RollerShutter::moveDown(){  
  if (!this->isMovingDown()) {
    /* Halt first */
    this->halt();
    
    digitalWrite(this->upDownPin, ROLLERSHUTTER_DOWN);
    delay(ROLLERSHUTTER_RELAY_DELAY); /* delay to make sure relais is switched correctly */
    digitalWrite(this->livePin, ROLLERSHUTTER_ON);
        
    this->movingDown = true;
    this->moveDownMillis = millis();
  }
}
void RollerShutter::moveUp(){
  if (!this->isMovingUp()) {
    /* Halt first */
    this->halt();
    
    digitalWrite(this->upDownPin, ROLLERSHUTTER_UP);
    delay(ROLLERSHUTTER_RELAY_DELAY); /* delay to make sure relais is switched correctly */
    digitalWrite(this->livePin, ROLLERSHUTTER_ON);
        
    this->movingUp = true;
    this->moveUpMillis = millis();
  }
}
void RollerShutter::halt(){
  if (this->isMovingUp() || this->isMovingDown()) {
    /* Update position if position is known */    
    if (!this->positionUnknown) {
      if (this->isMovingDown()){
        /* Moving down */
        unsigned long movedDuration = millis() - this->moveDownMillis;

        /* Substract move duration from current position */
        if (movedDuration > this->currentPosition) {
          this->currentPosition = 0;
        } else {
          this->currentPosition = (this->currentPosition - movedDuration);
        }
      } else {
        /* Moving up */
        unsigned long movedDuration = millis() - this->moveUpMillis;

        /* Add move duration to current position */
        if ((this->currentPosition + movedDuration) > this->moveDuration) {
          this->currentPosition = this->moveDuration;
        } else {
          this->currentPosition = (this->currentPosition + movedDuration);
        }
      } 
    }

    /* Control roller shutters */
    digitalWrite(this->livePin, ROLLERSHUTTER_OFF); /* stop roller shutter */
    delay(ROLLERSHUTTER_RELAY_DELAY);
    digitalWrite(this->upDownPin, LOW); /* disable updown relais */  
  
    /* Update state */
    this->movingUp = false;
    this->movingDown = false;   
    this->stopPosition = -1;    

    /* Call state listeners */    
    if (!this->positionUnknown){ 
      Serial.println("Halted. Position known");
      this->onStateChanged(); 
    } else {
      Serial.println("Halted. Position unkown");
    }
  }  
}

boolean RollerShutter::isMovingDown(){
  return this->movingDown;
}
boolean RollerShutter::isMovingUp(){
  return this->movingUp;
}

bool RollerShutter::addStateListener(IRollerShutterStateListener* listener){
  return this->stateListeners->add(listener);
}

void RollerShutter::onStateChanged() {
  double position = -1;
  if (!this->positionUnknown) { 
    if (this->currentPosition == 0) { position = 0 ;}
    else if (this->currentPosition == this->moveDuration) { position = 1;} 
    else { position = (double)this->currentPosition / (double)this->moveDuration;}
  }
  for (int i = 0; i < this->stateListeners->size(); i++) {
    IRollerShutterStateListener* listener =  this->stateListeners->get(i);
    listener->onStateChanged(this, position);
  }
}
