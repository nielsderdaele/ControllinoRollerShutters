#ifndef IROLLERSHUTTER_H
#define IROLLERSHUTTER_H

class IRollerShutter {
  public:
    virtual ~IRollerShutter() {}

    virtual byte getId() = 0;

    virtual boolean isMovingDown() = 0;
    virtual boolean isMovingUp() = 0;    
    
    virtual void moveDown() = 0;
    virtual void moveUp() = 0;
    virtual void halt() = 0;
};


#endif

