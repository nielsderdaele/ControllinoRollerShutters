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
    /* cut off if overflow or duration has expired */
    if ((currentMillis < moveUpMillis) || (currentMillis - moveUpMillis) >= this->moveDuration) {
      /* moved longer than max move duration */
      this->halt();      
      
      /* if position was unknown => configure it */
      if (this->positionUnknown) {
        this->positionUnknown = false;
        this->currentPosition = this->moveDuration; /* set to max */
      }
    }    
  } else if (this->isMovingDown()) {
    unsigned long currentMillis = millis();
    
    if ((currentMillis < moveDownMillis) || (currentMillis - moveDownMillis) >= this->moveDuration) {
      /* moved longer than max move duration */
      this->halt();

      /* if position was unknown => configure it */
      if (this->positionUnknown) {
        this->positionUnknown = false;
        this->currentPosition = 0; /* set to min */
      }
    }    
  }
}

byte RollerShutter::getId() {
  return this->id;
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
    /* Update position */
    if (!this->positionUnknown) {
      
    }


    digitalWrite(this->livePin, ROLLERSHUTTER_OFF); /* stop roller shutter */
    delay(ROLLERSHUTTER_RELAY_DELAY);
    digitalWrite(this->upDownPin, LOW); /* disable updown relais */  
  
    this->movingUp = false;
    this->movingDown = false;    
  }  
}

boolean RollerShutter::isMovingDown(){
  return this->movingDown;
}
boolean RollerShutter::isMovingUp(){
  return this->movingUp;
}
