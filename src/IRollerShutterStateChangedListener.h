#ifndef IRollerShutterStateChangedListener_H
#define IRollerShutterStateChangedListener_H

class IRollerShutterStateChangedListener  {
  public:
    virtual ~IRollerShutterStateChangedListener() {}
    virtual void onStateChanged(void*, int) = 0;     
};


#endif

