#ifndef IRollerShutterStateListener_H
#define IRollerShutterStateListener_H

class IRollerShutterStateListener  {
  public:
    virtual ~IRollerShutterStateListener() {}
    virtual void onStateChanged(void*, double) = 0;     
};


#endif

