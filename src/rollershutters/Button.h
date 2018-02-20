#include "IButtonListener.h"
#include "ISetupable.h"
#include "ILoopable.h"
#include <Arduino.h>
#include <LinkedList.h>
#ifndef BUTTON_H 
#define BUTTON_H

#define BUTTON_DEBOUNCE_MS 50
#define BUTTON_HOLD_MS 700
#define BUTTON_MULTICLICK_INTERVAL_MS 400
#define BUTTON_PRESSED HIGH
#define BUTTON_NOT_PRESSED LOW

class Button : public ISetupable, public ILoopable {
  private: 
    byte pin;
    byte state;
    unsigned long stateChangeMillis;
    unsigned long pressedMillis;    
    unsigned long clickedMillis;
    boolean isHolding;
    int numberOfClicks;
    
    IButtonListener *listener;
    LinkedList<IButtonListener*> *listeners;
    
    void checkButtonEvents();
    void checkStateChange();
    void onStateChanging(byte);
    
    void onClick();  
    void onHold();  
    void onRelease();      
  public:  
    Button(byte);
    void setup();
    void loop();    
    
    void setButtonListener(IButtonListener*);
    void addButtonListener(IButtonListener*);
};

#endif
