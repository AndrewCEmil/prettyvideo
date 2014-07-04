#ifndef __DAGCONTAINER_H_INCLUDED__
#define __DAGCONTAINER_H_INCLUDED__

#include "ppix.h"
#include "triops.h"
#include "triopgenerator.h"

class trigenerator;
class DAGContainer {
  public:
    DAGContainer(std::vector<trigenerator *> gens, triop *inpixel);
    std::vector<trigenerator *> generators;
    triop *pixel;
    void execute(double curX, double curY, double curT);
    DAGContainer *copyDAG();
};
#endif
