#include "triops.h"
#include "dagcontainer.h"

//TODO pull into the inline member setting
DAGContainer::DAGContainer(std::vector<trigenerator *> gens, triop *inpixel) {
    generators = gens;
    pixel = inpixel;
}
