#ifndef __TRIOPS_H_INCLUDED__
#define __TRIOPS_H_INCLUDED__

#include "triops.h"
#include "ppix.h"

class triedge;
class trival;
class triop {
  public:
    std::string name;
    int id;
    int num_args;
    std::vector<triedge *> myInputs;
    std::vector<triedge *> myOutputs;
    std::vector<std::shared_ptr<trival>> inputVals;
    virtual void execute() = 0;
    virtual std::string get_shader_string() { return ""; };
    triop *copyMe();
    static int getId();
    void checkAndExecute();
    std::string get_subordinate_shader_string();
  private:
    static int nextId;
};

//Outputnode -> inputNode
class triedge {
  public:
    triedge(triop *in, triop *out);
    triop *inputNode;  
    triop *outputNode;  
};

class tridouble : public triop {
  public:
    tridouble();
    void execute();
    std::string get_shader_string();
};

class trimult : public triop {
  public:
    trimult();
    void execute();
    std::string get_shader_string();
};

class tridiv : public triop {
  public:
    tridiv();
    void execute();
    std::string get_shader_string();
};

class trisquare : public triop {
  public:
    trisquare();
    void execute();
};

class triroot : public triop {
  public:
    triroot();
    void execute();
};

class trisin : public triop {
  public:
    trisin();
    void setPhaseFreq(double phase, double freq);
    void execute();
    double _phase;
    double _freq;
};

class trihighest : public triop {
  public:
    trihighest();
    void execute();
};

class trilevel : public triop {
  public:
    trilevel();
    void setThreshold(double threshold);
    void execute();
    double _threshold;
};

class triavg : public triop {
  public:
    triavg();
    void execute();
};

class trimod : public triop {
  public:
    trimod();
    void execute();
};

class triwell : public triop {
  public:
    triwell();
    void execute();
};

class tritent : public triop {
  public:
    tritent();
    void execute();
};

class trirgbtoycrcb : public triop {
  public:
    trirgbtoycrcb();
    void execute();
};

class triycrcbtorgb : public triop {
  public:
    triycrcbtorgb();
    void execute();
};

class tridis : public triop {
  public:
    tridis();
    void execute();
    std::string get_shader_string();
};

class tripic : public triop {
  private:
    BMP *_pic;
  public:
    //TODO do we need another constructor here?
    tripic();
    void execute();
};

class tripixel : public triop {
  public:
    tripixel();
    std::shared_ptr<trival> pixelVal;
    void execute();
    std::string get_shader_string();
};

class trigenerator : public triop {
   public:
     void setState(double curX, double curY, double curT);
     double _curX;
     double _curY;
     double _curT;
};

class trix : public trigenerator {
  public:
    trix();
    void execute();
    std::string get_shader_string();
};

class triy : public trigenerator {
  public:
    triy();
    void execute();
    std::string get_shader_string();
};

class trit : public trigenerator {
  public:
    trit();
    void execute();
    std::string get_shader_string();
};

class triconstpic : public trigenerator {
  private:
    BMP *_pic;
  public:
    //TODO do we need another constructor here?
    triconstpic();
    void execute();
};

class triconst : public trigenerator {
  public:
    triconst();
    void setConstri(std::shared_ptr<trival> constri);
    std::shared_ptr<trival> _constri;
    void execute();
};

class trirand : public trigenerator {
  public:
    trirand();
    void execute();
};


#endif
