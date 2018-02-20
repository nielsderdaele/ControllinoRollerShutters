#include "ISetupable.h"
#include "ILoopable.h"
#ifndef IPROGRAM_H
#define IPROGRAM_H

class IProgram : public ILoopable, public ISetupable {
  public:
    virtual ~IProgram() {}
};


#endif

