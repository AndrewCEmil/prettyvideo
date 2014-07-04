#include "triopgenerator.h"

int triop_generator::get_num_args(std::string op_name) {
    int num_args = -1;
    if(op_name.compare("tridouble") == 0) {
        num_args = 1;
    } else if(op_name.compare("trisquare") == 0) {
        num_args = 1;
    } else if(op_name.compare("triroot") == 0) {
        num_args = 1;
    } else if(op_name.compare("trisin") == 0) {
        num_args = 1;
    } else if(op_name.compare("trihighest") == 0) {
        num_args = 2;
    } else if(op_name.compare("trilevel") == 0) {
        num_args = 3;
    } else if(op_name.compare("triavg") == 0) {
        num_args = 2;
    } else if(op_name.compare("triconst") == 0) {
        num_args = 0;
    } else if(op_name.compare("trirand") == 0) {
        num_args = 0;
    } else if(op_name.compare("trisum") == 0) {
        num_args = 2;
    } else if(op_name.compare("trimult") == 0) {
        num_args = 2;
    } else if(op_name.compare("trimod") == 0) {
        num_args = 2;
    } else if(op_name.compare("triwell") == 0) {
        num_args = 1;
    } else if(op_name.compare("tritent") == 0) {
        num_args = 1;
    } else if(op_name.compare("trirgbtoycrcb") == 0) {
        num_args = 1;
    } else if(op_name.compare("triycrcbtorgb") == 0) {
        num_args = 1;
    } else if(op_name.compare("tridiv") == 0) {
        num_args = 2;
    } else if(op_name.compare("tridis") == 0) {
        num_args = 3;
    } else if(op_name.compare("tripic") == 0) {
        num_args = 2;
    } else if(op_name.compare("tripixel") == 0) {
        num_args = 1;
    } else if(op_name.compare("triconstpic") == 0) {
        num_args = 0;
    } else if(op_name.compare("trix") == 0) {
        num_args = 0;
    } else if(op_name.compare("triy") == 0) {
        num_args = 0;
    } else if(op_name.compare("trit") == 0) {
        num_args = 0;
    } else {
        std::cout<<"UNKNOWN TRI OP TO Name"<<std::endl;
        std::cout<<op_name<<std::endl;
    }
    return num_args;
}

//triop *triop_generator::generate(std::vector<double> inputs) {
triop *triop_generator::generate_op(std::string op_name) {
    if(op_name.compare("tridouble") == 0) {
        tridouble *ret_op = new tridouble();
        return ret_op;
    } else if(op_name.compare("trisquare") == 0) {
        trisquare *ret_op = new trisquare();
        return ret_op;
    } else if(op_name.compare("triroot") == 0) {
        triroot *ret_op = new triroot();
        return ret_op;
    } else if(op_name.compare("trisin") == 0) {
        trisin *ret_op = new trisin();
        return ret_op;
    } else if(op_name.compare("trihighest") == 0) {
        trihighest *ret_op = new trihighest();
        return ret_op;
    } else if(op_name.compare("trilevel") == 0) {
        trilevel *ret_op = new trilevel();
        return ret_op;
    } else if(op_name.compare("triavg") == 0) {
        triavg *ret_op = new triavg();
        return ret_op;
    } else if(op_name.compare("trimod") == 0) {
        trimod *ret_op = new trimod();
        return ret_op;
    } else if(op_name.compare("trimult") == 0) {
        trimult *ret_op = new trimult();
        return ret_op;
    } else if(op_name.compare("triwell") == 0) {
        triwell *ret_op = new triwell();
        return ret_op;
    } else if(op_name.compare("tritent") == 0) {
        tritent *ret_op = new tritent();
        return ret_op;
    } else if(op_name.compare("trirgbtoycrcb") == 0) {
        trirgbtoycrcb *ret_op = new trirgbtoycrcb();
        return ret_op;
    } else if(op_name.compare("triycrcbtorgb") == 0) {
        triycrcbtorgb *ret_op = new triycrcbtorgb();
        return ret_op;
    } else if(op_name.compare("tridiv") == 0) {
        tridiv *ret_op = new tridiv();
        return ret_op;
    } else if(op_name.compare("tridis") == 0) {
        tridis *ret_op = new tridis();
        return ret_op;
    } else if(op_name.compare("tripic") == 0) {
        tripic *ret_op = new tripic();
        return ret_op;
    } else if(op_name.compare("trix") == 0) {
        trix *ret_op = new trix();
        return ret_op;
    } else if(op_name.compare("triconstpic") == 0) {
        triconstpic *ret_op = new triconstpic();
        return ret_op;
    } else if(op_name.compare("triy") == 0) {
        triy *ret_op = new triy();
        return ret_op;
    } else if(op_name.compare("trit") == 0) {
        trit *ret_op = new trit();
        return ret_op;
    } else if(op_name.compare("triconst") == 0) {
        triconst *ret_op = new triconst();
        return ret_op;
    } else if(op_name.compare("trirand") == 0) {
        trirand *ret_op = new trirand();
        return ret_op;
    } else if(op_name.compare("tripixel") == 0) {
        tripixel *ret_op = new tripixel();
        return ret_op;
    } 
    std::cout<<"UNKNOWN DOUBLE OP TO GENERATE"<<std::endl;
    std::cout<<op_name<<std::endl;
    return NULL;
}   
