#include <string>
#include <stack>
#include <set>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <thread>
#include <sstream>
#include "EasyBMP.h"
#include "ppix.h"
#include "triops.h"
#include "triopgenerator.h"

using std::shared_ptr;

int triop::nextId = 0;

int triop::getId() {
    int retId = nextId;
    nextId++;
    return retId;
}

//TODO pull into the inline member setting
triedge::triedge(triop *in, triop *out) {
    inputNode = in;
    outputNode = out;
}

triop *triop::copyMe() {
    triop *mycopy = triop_generator::generate_op(name);
    //make a new copy
    for(int i = 0; i < myInputs.size(); i++) {
        //recur
        triop *inputCopy = myInputs[i]->outputNode->copyMe();
        triedge *edgecopy = new triedge(mycopy, inputCopy);
        //append results to mycopy
        mycopy->myInputs.push_back(edgecopy);
        inputCopy->myOutputs.push_back(edgecopy);
    }

    //set paramenters if needed
    if(name.compare("trisin") == 0) {
        ((trisin *) mycopy)->setPhaseFreq(((trisin *)this)->_phase, ((trisin *) this)->_freq);
    } else if(name.compare("triconst") == 0) {
        triconst *mythis = (triconst *)this;
        ((triconst *)mycopy)->setConstri(mythis->_constri);
    } else if(name.compare("trilevel") == 0) {
        trilevel *mythis = (trilevel *)this;
        ((trilevel *)mycopy)->setThreshold(mythis->_threshold);
    }
    return mycopy;
}

void triop::checkAndExecute() {
    if(this->inputVals.size() == this->num_args) {
        this->execute();
    }   
}


trival::trival(double inr, double ing, double inb) {
    r = inr;
    g = ing;
    b = inb;
}

tridouble::tridouble() {
    name = "tridouble";
    id = getId();
    num_args = 1;
}

void tridouble::execute() {
    std::shared_ptr<trival> inval = inputVals[0];
    double rval = inval->r * 2.0;
    double gval = inval->g * 2.0;
    double bval = inval->b * 2.0;
    std::shared_ptr<trival> res(new trival(rval, gval, bval));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trimult::trimult() {
    name = "trimult";
    id = getId();
    num_args = 2;
}

void trimult::execute() {
    std::shared_ptr<trival> inval0 = inputVals[0];
    std::shared_ptr<trival> inval1 = inputVals[1];
    std::shared_ptr<trival> res(new trival(inval0->r * inval1->r,
                                           inval0->g * inval1->g,
                                           inval0->b * inval1->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

tridiv::tridiv() {
    name = "tridiv";
    id = getId();
    num_args = 2;
}

void tridiv::execute() {
    std::shared_ptr<trival> inval0 = inputVals[0];
    std::shared_ptr<trival> inval1 = inputVals[1];
    std::shared_ptr<trival> res(new trival(inval0->r / inval1->r,
                                           inval0->g / inval1->g,
                                           inval0->b / inval0->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

tridis::tridis() {
    name = "tridis";
    id = getId();
    num_args = 3;
}

void tridis::execute() {
    std::shared_ptr<trival> factor = inputVals[0];
    std::shared_ptr<trival> i0 = inputVals[1];
    std::shared_ptr<trival> i1 = inputVals[2];
    double rval = i0->r * abs(factor->r) + i1->r * (1 - abs(factor->r));
    double gval = i0->g * abs(factor->g) + i1->g * (1 - abs(factor->g));
    double bval = i0->b * abs(factor->b) + i1->b * (1 - abs(factor->b));
    std::shared_ptr<trival> res(new trival(rval, gval, bval));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}


triconst::triconst() {
    name = "triconst";
    id = getId();
    //three values between -1 and 1
    double randr = ((double) std::rand() / (RAND_MAX));
    double randg = ((double) std::rand() / (RAND_MAX));
    double randb = ((double) std::rand() / (RAND_MAX));
    std::cout << "randr: " << randr;
    std::cout << "randg: " << randg;
    std::cout << "randb: " << randb;

    _constri = shared_ptr<trival>((new trival(randr, randg, randb)));
    num_args = 0;
}

void triconst::setConstri(std::shared_ptr<trival> constri) {
    _constri = constri;
}

void triconst::execute() {
    myOutputs[0]->inputNode->inputVals.push_back(_constri);
    myOutputs[0]->inputNode->checkAndExecute();
}

trirand::trirand() {
    name = "trirand";
    id = getId();
    num_args = 0;
}


void trirand::execute() {
    double randr = ((double) std::rand() / (RAND_MAX));
    double randg = ((double) std::rand() / (RAND_MAX));
    double randb = ((double) std::rand() / (RAND_MAX));
    shared_ptr<trival> res(new trival(randr, randg, randb));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
}

void trigenerator::setState(double curX, double curY, double curT) {
    _curX = curX;
    _curY = curY;
    _curT = curT;
}

trix::trix() {
    name = "trix";
    id = getId();
    num_args = 0;
}

void trix::execute() {
    std::shared_ptr<trival> xval(new trival(_curX, _curX, _curX));
    myOutputs[0]->inputNode->inputVals.push_back(xval);
    myOutputs[0]->inputNode->checkAndExecute();
}

triconstpic::triconstpic() {
    name = "triconstpic";
    _pic = new BMP();
    _pic->ReadFromFile("/Users/ace/ppix/copied/cpp/nebula.bmp");
    id = getId();
    num_args = 0;
}

void triconstpic::execute() {
    int realX = floor(_curX * _pic->TellWidth());
    int realY = floor(_curY * _pic->TellHeight());
    double rval = _pic->GetPixel(realX,realY).Red / 256.0;
    double gval = _pic->GetPixel(realX,realY).Green / 256.0;
    double bval = _pic->GetPixel(realX,realY).Blue / 256.0;
    /*
    std::cout << "width: " << _pic->TellWidth() << std::endl;
    std::cout << "height: " << _pic->TellHeight() << std::endl;
    std::cout << "curX: " << curX << std::endl;
    std::cout << "curY: " << curY << std::endl;
    std::cout << "realX: " << realX << std::endl;
    std::cout << "realY: " << realY << std::endl;
    std::cout << "rval: " << rval << std::endl;
    */
    std::shared_ptr<trival> res(new trival(rval, gval, bval));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
}

tripic::tripic() {
    name = "tripic";
    _pic = new BMP();
    _pic->ReadFromFile("/Users/ace/ppix/copied/cpp/nebula.bmp");
    id = getId();
    num_args = 2;
}

void tripic::execute() {
    std::shared_ptr<trival> x = inputVals[0];
    std::shared_ptr<trival> y = inputVals[1];
    //get coordinates in the image
    double rval = _pic->GetPixel(abs(x->r), abs(y->r)).Red / 256.0;
    double gval = _pic->GetPixel(abs(x->g), abs(y->g)).Green / 256.0;
    double bval = _pic->GetPixel(abs(x->b), abs(y->b)).Blue / 256.0;
    std::shared_ptr<trival> xval(new trival(rval, gval, bval));
    myOutputs[0]->inputNode->inputVals.push_back(xval);
    myOutputs[0]->inputNode->checkAndExecute();
}

triy::triy() {
    name = "triy";
    id = getId();
    num_args = 0;
}

void triy::execute() {
    std::shared_ptr<trival> yval(new trival(_curY, _curY, _curY));
    myOutputs[0]->inputNode->inputVals.push_back(yval);
    myOutputs[0]->inputNode->checkAndExecute();
}

trit::trit() {
    name = "trit";
    id = getId();
    num_args = 0;
}

void trit::execute() {
    std::shared_ptr<trival> tval(new trival(_curT, _curT, _curT));
    myOutputs[0]->inputNode->inputVals.push_back(tval);
    myOutputs[0]->inputNode->checkAndExecute();
}


trisquare::trisquare() {
    name = "trisquare";
    id = getId();
    num_args = 1;
}

void trisquare::execute() {
    std::shared_ptr<trival> inval = inputVals[0];
    std::shared_ptr<trival> res(new trival(inval->r * inval->r,
                                           inval->g * inval->g,
                                           inval->b * inval->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

triroot::triroot() {
    name = "triroot";
    id = getId();
    num_args = 1;
}

void triroot::execute() {
    std::shared_ptr<trival> inval = inputVals[0];
    std::shared_ptr<trival> res(new trival(sqrt(inval->r),
                                           sqrt(inval->g),
                                           sqrt(inval->b)));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trisin::trisin() {
    name = "trisin";
    id = getId();
    num_args = 1;
    _phase = ((double) std::rand() / (RAND_MAX)) * M_PI;
    _freq = ((double) std::rand() / (RAND_MAX))*5 + 1;
}

void trisin::setPhaseFreq(double phase, double freq) {
    _phase = phase;
    _freq = freq;
}

void trisin::execute() {
    std::shared_ptr<trival> inval = inputVals[0];
    std::shared_ptr<trival> res(new trival(_phase + _freq * sin(inval->r),
                                           _phase + _freq * sin(inval->g),
                                           _phase + _freq * sin(inval->b)));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trihighest::trihighest() {
    name = "trihighest";
    id = getId();
    num_args = 2;
}

void trihighest::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> i1 = inputVals[1];
    std::shared_ptr<trival> res(new trival(i0->r > i1->r ? i0->r : i1->r,
                           i0->g > i1->g ? i0->g : i1->g,
                           i0->b > i1->b ? i0->b : i1->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trilevel::trilevel() {
    name = "trilevel";
    id = getId();
    num_args = 3;
    _threshold = ((double) std::rand() / (RAND_MAX));
}

void trilevel::setThreshold(double threshold) {
    _threshold = threshold;
}

void trilevel::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> i1 = inputVals[1];
    std::shared_ptr<trival> i2 = inputVals[1];
    std::shared_ptr<trival> res(new trival(i0->r < _threshold ? i1->r : i2->r,
                                           i0->g < _threshold ? i1->g : i2->g,
                                           i0->b < _threshold ? i1->b : i2->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

triavg::triavg() {
    name = "triavg";
    id = getId();
    num_args = 2;
}

void triavg::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> i1 = inputVals[1];
    std::shared_ptr<trival> res(new trival((i0->r + i1->r) / 2.0,
                                           (i0->g + i1->g) / 2.0,
                                           (i0->b + i1->b) / 2.0));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trimod::trimod() {
    name = "trimod";
    id = getId();
    num_args = 2;
}

void trimod::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> i1 = inputVals[1];
    std::shared_ptr<trival> res(new trival((fmod(i0->r, i1->r)),
                                           (fmod(i0->g, i1->g)),
                                           (fmod(i0->b, i1->b))));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

triwell::triwell() {
    name = "triwell";
    id = getId();
    num_args = 1;
}

void triwell::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> res(new trival((1 - ((2 / (1 + i0->r * i0->r)) * 8)),
                                           (1 - ((2 / (1 + i0->g * i0->g)) * 8)),
                                           (1 - ((2 / (1 + i0->g * i0->b)) * 8))));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

tritent::tritent() {
    name = "tritent";
    id = getId();
    num_args = 1;
}

void tritent::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> res(new trival((1 - (2 * abs(i0->r))),
                                           (1 - (2 * abs(i0->g))),
                                           (1 - (2 * abs(i0->b)))));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

trirgbtoycrcb::trirgbtoycrcb() {
    name = "trirgbtoycrcb";
    id = getId();
    num_args = 1;
}

void trirgbtoycrcb::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> res(new trival(0.2989 * i0->r + 0.5866 * i0->g + 0.1145 * i0->b,
                                           0.5 * i0->r - 0.4183 * i0->g - 0.0816 * i0->b,
                                           -0.1687 * i0->r - 0.3312 * i0->g + 0.5 * i0->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

triycrcbtorgb::triycrcbtorgb() {
    name = "triycrcbtorgb";
    id = getId();
    num_args = 1;
}

void triycrcbtorgb::execute() {
    std::shared_ptr<trival> i0 = inputVals[0];
    std::shared_ptr<trival> res(new trival(i0->r + 1.4022 * i0->g,
                                           -1.0 * i0->r - 0.7145 * i0->g - 0.3456 * i0->b,
                                           0.5 * i0->r + 1.7710 * i0->b));
    myOutputs[0]->inputNode->inputVals.push_back(res);
    myOutputs[0]->inputNode->checkAndExecute();
    inputVals.clear();
}

tripixel::tripixel() {
    name = "tripixel";
    id = getId();
    num_args = 1;
}

void tripixel::execute() {
    //just set the pixelVal
    pixelVal = inputVals[0];
    inputVals.clear();
}
