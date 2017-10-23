#ifndef IBUTTONLISTENER_H
#define IBUTTONLISTENER_H

class IButtonListener {
  public:
    virtual ~IButtonListener() {}
    virtual void onButtonClicked(void*, int) = 0; 
    virtual void onButtonHold(void*) = 0; 
    virtual void onButtonRelease(void*) = 0; 
};


#endif

