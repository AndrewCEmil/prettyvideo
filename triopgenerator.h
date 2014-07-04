#ifndef __TRIOPGENERATOR_H_INCLUDED__
#define __TRIOPGENERATOR_H_INCLUDED__

#include "ppix.h"
#include "triops.h"

class triop;
class triop_generator {
    public:
      static int get_num_args(std::string op_name);
      static triop *generate_op(std::string op_name);
};
#endif
