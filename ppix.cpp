#include <string>
#include <stack>
#include <set>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <thread>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "EasyBMP.h"
#include "ppix.h"
#include "triops.h"
#include "triopgenerator.h"
#include "dagcontainer.h"
#include "daggenerator.h"

using std::shared_ptr;
using namespace std;

int nextId = 0;
int itercounter = 0;
int MAX_DEPTH = 12;
//debug functions

void printSpaces(int spaces) {
    for(int i = 0; i < spaces; i++) {
        cout<<" ";
    }
}

void printAllOps(vector<triop *> allOps) {
    int numInputs = 0;
    for(int i = 0; i < allOps.size(); i++) {
        cout << allOps[i]->name << ":" << allOps[i]->id << endl;
        cout << "num_args: " << allOps[i]->num_args << ", inputsize: " << allOps[i]->myInputs.size() << endl;
        numInputs += allOps[i]->num_args;
    }
    cout << "NumInputs: " << numInputs << endl;
}

void printIOCount(vector<triop *> allOps) {
    int incount = 0;
    int realincount = 0;
    int outcount = 0;
    int realoutcount = 0;
    triop *curOp;
    for(int i = 0; i < allOps.size(); i++) {
        curOp = allOps[i];
        incount += curOp->num_args;
        realincount += curOp->myInputs.size();
        realoutcount += curOp->myOutputs.size();
    }
    cout << "incount: " << incount << endl;
    cout << "outcount: " << outcount << endl;
    cout << "realincount: " << realincount << endl;
    cout << "realoutcount: " << realoutcount << endl;
}

void visitPrint(triop *curNode, int curDepth) {
    printSpaces(curDepth);
    cout << curNode->name << ":" << curNode->id << endl;
    curDepth++;
    for(int i = 0; i < curNode->num_args; i++) {
        visitPrint(curNode->myInputs[i]->outputNode, curDepth);
    }
}

void printDag(DAGContainer *dag) {
    visitPrint(dag->pixel, 0);
}

//DONE with debug

vector<string> getAllOps() {
    vector<string> out;
    out.push_back("trisin");
    out.push_back("trisin");
    out.push_back("trimult");
    out.push_back("triavg");
    out.push_back("trimod");
    out.push_back("triwell");
    out.push_back("tritent");
    out.push_back("trilevel");

    out.push_back("triycrcbtorgb");
    out.push_back("trirgbtoycrcb");
    out.push_back("tridis");
    return out;
}

vector<string> getAllDims() {
    vector<string> out;
    //out.push_back("triconstpic");

    //out.push_back("trit");
    out.push_back("trit");

    out.push_back("triy");
    out.push_back("trix");
    return out;
}

void fillGenerators(triop *curOp, vector<trigenerator *> &generators) {
    if(curOp->num_args == 0) {
        generators.push_back((trigenerator *)curOp);
    } else {
        for(int i = 0; i < curOp->myInputs.size(); i++) {
            fillGenerators(curOp->myInputs[i]->outputNode, generators);
        }
    }
}

DAGContainer *DAGContainer::copyDAG() {
    //Copy tree from root
    triop *newroot = pixel->copyMe();
    //find and save all of the generators
    vector<trigenerator *> newgenerators;
    fillGenerators(newroot, newgenerators);
    //construct new dag and return
    DAGContainer *newDAG = new DAGContainer(newgenerators, newroot);
    return newDAG;
}

void DAGContainer::execute(double curX, double curY, double curT) {
    for(int i = 0; i < generators.size(); i++) {
        generators[i]->setState(curX, curY, curT);
        generators[i]->execute();
    }
}

int getIdx(vector<triop *> ops, triop *targetOp) {
    for(int i = 0; i < ops.size(); i++) {
        if(ops[i] == targetOp)
            return i;
    }
    return -1;
}

bool useleaf(int curDepth) {
    if(curDepth == MAX_DEPTH)
        return true;
    //a 1:(MAX_DEPTH - curDepth) chance of being a leaf
    int mynum = (rand() % (MAX_DEPTH - curDepth));
    if(mynum == 0)
        return true;
    return false;
}

void recurTree(triop *curNode, int curDepth, vector<string> generators, vector<string> ops, vector<trigenerator *> &dimOps) {
    int randidx;
    triop *nextNode;
    triedge *nexttriedge;
    for(int i = 0; i < curNode->num_args; i++) {
        if(useleaf(curDepth)) {
            randidx = rand() % generators.size();
            nextNode = triop_generator::generate_op(generators[randidx]);
            //TODO bad casting, refactor
            dimOps.push_back((trigenerator *)nextNode);
        } else {
            randidx = rand() % ops.size();
            nextNode = triop_generator::generate_op(ops[randidx]);
            recurTree(nextNode, curDepth + 1, generators, ops, dimOps);
        }

        nexttriedge = new triedge(curNode, nextNode);
        curNode->myInputs.push_back(nexttriedge);
        nextNode->myOutputs.push_back(nexttriedge);
    }
    return;
}

DAGContainer *generateTREE() {
    vector<string> allOps = getAllOps();
    vector<string> generators = getAllDims();
    vector<trigenerator *> dimOps;
    
    triop *root = new tripixel();
    recurTree(root, 0, generators, allOps, dimOps);

    DAGContainer *tree = new DAGContainer(dimOps, root);
    return tree;
}

//END generation section


void video_generator::createImageRange(DAGContainer *dag, int width, int height, int startT, int endT, int maxT, int bitdepth) {
    double curT;
    image_generator *ig = new image_generator(width, height);
    for(int myt = startT; myt < endT; myt++) {
        //curT =(( 2.0 * myt )/ ((double) maxT)) - 1;
        curT = myt / ((double) maxT);
        BMP curImage;
        char imgName[13];
        curImage.SetSize(width, height);
        curImage.SetBitDepth(bitdepth);
        cout << "Calling generateFromDAG with curT: " << curT << endl;
        ig->generateFromDAG(dag, curImage, curT);
        sprintf (imgName, "img%05d.bmp", myt);
        curImage.WriteToFile(imgName);
    }
}

image_generator::image_generator(int width, int height) {
    _width = width;
    _height = height;
}

void image_generator::generateFromDAG(DAGContainer *dag, BMP &image, double curT) {
    int redValue = 0;
    int greenValue = 0;
    int blueValue = 0;
    double curX;
    double curY;
    cout << "Generating from DAG with curT: " << curT << endl;
    for(int i=0;i<_width;i++) {
        for(int j=0;j<_height;j++) {
            curX = (((double)i)/((double)_width));
            curY = (((double)j)/((double)_height));

            dag->execute(curX, curY, curT);

            redValue = ((int) (((tripixel *) dag->pixel)->pixelVal->r  * 255.0)) % 256;
            greenValue = ((int) (((tripixel *) dag->pixel)->pixelVal->g  * 255.0)) % 256;
            blueValue = ((int) (((tripixel *) dag->pixel)->pixelVal->b  * 255.0)) % 256;

            image(i,j)->Red = redValue;
            image(i,j)->Green = greenValue;
            image(i,j)->Blue = blueValue;

            image(i,j)->Alpha = 0;
        }
    }
}

video_generator::video_generator(int width, int height, int framecount, int threads, int bitdepth) {
    _width = width;
    _height = height;
    _framecount = framecount;
    _threads = threads;
    _bitdepth = bitdepth;
}

string shader_generator::generateFromDAG(DAGContainer *dag) {
    ostringstream ss;
    ss << "precision mediump float;" << endl << "varying vec2 position;" << endl << "void main() {" << endl;
    string startstring = ss.str();
    ostringstream es;
    es << "gl_FragColor.a = 1.0;" << endl << "}" << endl;
    string endstring = es.str();
    string programstring = dag->pixel->get_shader_string();
    startstring += programstring;
    startstring += endstring;
    return startstring;
}

void video_generator::generateFromDAG(DAGContainer *dag) {
    //TODO chop out and replace with image_generator
    int frameSection = _framecount / _threads;
    int framestart = 0;
    vector<thread> threads;
    for(int i = 0; i < _threads; i++) {
        cout << "Section: " << framestart << endl;
        DAGContainer *copiedDag = dag->copyDAG();
        threads.push_back(thread(createImageRange, copiedDag, _width, _height, framestart, framestart + frameSection, _framecount, _bitdepth));
        framestart += frameSection;
    }
    for (auto& th : threads) th.join();
    
}



int main(int argc, char **argv) {
    int opt;
    char *inval = NULL;
    int num_threads = 8;
    int width = 512;
    int height = 512;
    int framecount = 256;
    int bitdepth = 32;
    int seed = time(NULL);
    bool make_shader = false;
    string dagstring = "tripixel triavg trix trit";
    DAGContainer *dag = NULL;
    while ((opt = getopt(argc, argv, "w:h:f:t:b:s:d:i:rm")) != -1) {
      cout << "processing: " << (char)opt << endl;
      switch (opt) {
        case 'w':
          width = atoi(optarg);
          break;
        case 'h':
          height = atoi(optarg);
          break;
        case 'f':
          framecount = atoi(optarg);
          break;
        case 't':
          num_threads = atoi(optarg);
          break;
        case 'b':
          bitdepth = atoi(optarg);
          break;
        case 's':
          seed = atoi(optarg);
          break;
        case 'd':
          dagstring = string(optarg);
          break;
        case 'r':
          srand(seed);
          dag = generateTREE();
          break;
        case 'm':
          make_shader = true;
          break;
        default:
          cout << "Unknown flag" << endl;
          exit(1);
      }
    }
    srand(seed);
    double curT;
    //first get dag
    cout << "Generating TREE" << endl;
    if(dag == NULL) {
        dag = DAGGenerator::generateFromString(dagstring);
    }
    printDag(dag);
    if(make_shader) {
        shader_generator *sg = new shader_generator();
        string shaderstr = sg->generateFromDAG(dag);
        cout << shaderstr << endl;
    } else {
        video_generator *vg = new video_generator(width, height, framecount, num_threads, bitdepth);
        vg->generateFromDAG(dag);
    }
}
