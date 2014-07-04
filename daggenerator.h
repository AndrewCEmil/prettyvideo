#ifndef __DAGGENERATOR_H_INCLUDED__
#define __DAGGENERATOR_H_INCLUDED__

#include "ppix.h"
#include "triops.h"
#include "triopgenerator.h"
#include "dagcontainer.h"

class DAGGenerator {
  public:
    static DAGContainer *generateFromString(std::string treeDef);
};
#endif
