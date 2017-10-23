#include <Arduino.h>
#include "Button.h"
#include "IRollerShutter.h"
#include "IButtonListener.h"
#ifndef BUTTONROLLERSHUTTER_H
#define BUTTONROLLERSHUTTER_H

class ButtonRollerShutter : IButtonListener {
  private:     
    Button *upButton;
    Button *downButton;
    IRollerShutter *rollerShutter;
    byte buttonClickEnabled;

    void onDownButtonClick(void);
    void onDownButtonHold(void);
    void onDownButtonRelease(void);
    
    void onUpButtonClick(void);
    void onUpButtonHold(void);
    void onUpButtonRelease(void);           
    
  public:  
    ButtonRollerShutter(IRollerShutter*, Button*, Button*,byte);
    void setup();    
    
    /*IButtonListener Implementation */
    void onButtonClicked(void*, int);
    void onButtonHold(void*); 
    void onButtonRelease(void*); 
};

#endif
