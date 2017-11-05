#include "Button.h"

Button::Button(byte pin) {
  this->pin = pin;    
  this->state = BUTTON_NOT_PRESSED;
  this->stateChangeMillis = 0;
  this->pressedMillis = 0;  
  this->clickedMillis = 0;
  this->isHolding = false;
  this->numberOfClicks = 0;
  this->listeners = new LinkedList<IButtonListener*>();
}

void Button::setup() {
  pinMode(this->pin, INPUT);
}
void Button::loop() {
  this->checkButtonEvents();
  this->checkStateChange();
}

void Button::addButtonListener(IButtonListener *listener) {
  this->listeners->add(listener);
}
void Button::setButtonListener(IButtonListener *listener) {
  this->listener = listener;
}

void Button::checkButtonEvents() {            
  /* Check if we are holding the button */  
  unsigned long currentMillis = millis();
  if (this->state == BUTTON_PRESSED && !this->isHolding && (currentMillis - this->pressedMillis) > BUTTON_HOLD_MS) {
    /* Button is pressed for longer than holding treshold */    
    this->onHold();
  }

  /* Check if we stopped clicking (multi click) */
  if (this->state == BUTTON_NOT_PRESSED && this->numberOfClicks > 0 && (currentMillis - this->clickedMillis) > BUTTON_MULTICLICK_INTERVAL_MS) {
    this->onClick(); /* Sent click event and reset number of clicks */
  }
}
void Button::checkStateChange() {            
    byte physicalState = digitalRead(this->pin); /* Up button*/
    unsigned long currentMillis = millis();
    
    if (physicalState != this->state) {
      if (this->stateChangeMillis == 0) {
        this->stateChangeMillis = currentMillis;        
      } else if (currentMillis - this->stateChangeMillis >= BUTTON_DEBOUNCE_MS) {                
        this->onStateChanging(physicalState);
        this->stateChangeMillis = 0;
        this->state = physicalState;        
      }
    } else if (this->stateChangeMillis != 0) {
      /* state reverted during debounce period => reset debounce period */
      this->stateChangeMillis = 0;      
    }
}
void Button::onStateChanging(byte state) {
  if (state == BUTTON_PRESSED) {
    this->pressedMillis = millis(); /* set pressed millis */
  } else if (state == BUTTON_NOT_PRESSED) {
    if (this->isHolding) {
      /* Button was released after holding it => remove holding flag and sent release event */      
      this->onRelease();
    } else {
      /* Button was released after being pressed => increase number of clicks & set millis */
      this->clickedMillis = millis();
      this->numberOfClicks++;
    }
  }
}

void Button::onClick() {
  /* Sent events */
  if (this->listener != 0) {
    this->listener->onButtonClicked(this, this->numberOfClicks);    
  }
  for (int i = 0; i < this->listeners->size(); i++){
    this->listeners->get(i)->onButtonClicked(this, this->numberOfClicks);
  }

  /* reset number of clicks after sending the event */
  this->numberOfClicks = 0;
}
void Button::onHold() {
  /* Set is holding flag */
  this->isHolding = true;

  /* Clear number of clicks */
  this->numberOfClicks = 0; 

  /* Sent events */
  if (this->listener != 0) {
    this->listener->onButtonHold(this);    
  }
  for (int i = 0; i < this->listeners->size(); i++){
    this->listeners->get(i)->onButtonHold(this);
  }
}
void Button::onRelease() {
  /* Remove holding flag */
  this->isHolding = false;
  
  /* Sent events */
  if (this->listener != 0) {
    this->listener->onButtonRelease(this);    
  }  
  for (int i = 0; i < this->listeners->size(); i++){
    this->listeners->get(i)->onButtonRelease(this);
  }
}