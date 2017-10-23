#include "Button.h"
#include "ButtonRollerShutter.h"

ButtonRollerShutter::ButtonRollerShutter(IRollerShutter *rollerShutter, Button *upButton, Button *downButton, byte buttonClickEnabled) {  
  this->rollerShutter = rollerShutter;
  this->upButton = upButton;
  this->downButton = downButton;
  this->buttonClickEnabled = buttonClickEnabled;

  this->upButton->setButtonListener(this);
  this->downButton->setButtonListener(this);
}

void ButtonRollerShutter::setup() {}


/* IButtonListener implementation */ 
void ButtonRollerShutter::onButtonClicked(void *sender, int numberOfClicks){
  if (this->buttonClickEnabled) {
    if (sender == this->downButton) {
      this->onDownButtonClick();        
    } else if (sender == this->upButton) {
      this->onUpButtonClick();
    }
  }
}

void ButtonRollerShutter::onButtonHold(void *sender) {
  if (sender == this->downButton) {
    this->onDownButtonHold();
  } else if (sender == this->upButton) {
    this->onUpButtonHold();
  }
}
void ButtonRollerShutter::onButtonRelease(void *sender){
  if (sender == this->downButton) {
    this->onDownButtonRelease();
  } else if (sender == this->upButton) {
    this->onUpButtonRelease();
  }
}


/* Button handlers */
void ButtonRollerShutter::onUpButtonClick() {
  if (this->rollerShutter->isMovingUp() || this->rollerShutter->isMovingDown()) {
    this->rollerShutter->halt();
  } else {
    this->rollerShutter->moveUp();
  }
}
void ButtonRollerShutter::onUpButtonRelease() {
  this->rollerShutter->halt();  
}
void ButtonRollerShutter::onUpButtonHold() {
  this->rollerShutter->moveUp();
}

void ButtonRollerShutter::onDownButtonClick() {  
  if (this->rollerShutter->isMovingUp() || this->rollerShutter->isMovingDown()) {
    this->rollerShutter->halt();
  } else {
    this->rollerShutter->moveDown();
  }
}
void ButtonRollerShutter::onDownButtonRelease() {  
  this->rollerShutter->halt();  
}
void ButtonRollerShutter::onDownButtonHold() {  
  this->rollerShutter->moveDown();
}
