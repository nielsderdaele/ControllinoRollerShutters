#ifndef ISETUPABLE_H
#define ISETUPABLE_H

class ISetupable {
  public:
    virtual ~ISetupable() {}
    virtual void setup() = 0; 
};


#endif

