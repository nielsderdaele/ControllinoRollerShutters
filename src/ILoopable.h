#ifndef ILOOPABLE_H
#define ILOOPABLE_H

class ILoopable {
  public:
    virtual ~ILoopable() {}
    virtual void loop() = 0; 
};


#endif

