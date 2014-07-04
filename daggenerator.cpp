#include "daggenerator.h"
#include "triopgenerator.h"
#include "triops.h"
#include <sstream>
#include <string>
#include <stack>

DAGContainer *DAGGenerator::generateFromString(std::string treeDef) {
    std::vector<std::string> nodeNames;
    std::stringstream ss(treeDef);
    std::string item;
    //tokenize
    while (std::getline(ss, item, ' ')) {
        nodeNames.push_back(item);
    }

    std::stack<triop *> nodestack;
    std::vector<triop *> readynodes;
    for(int i = 0; i < nodeNames.size(); i++) {
        nodestack.push(triop_generator::generate_op(nodeNames[i]));
    }

    triop *curNode;
    triop *inputNode;
    std::vector<trigenerator *> dimOps;
    std::stack<triop *> readyNodes;
    triedge *edge;
    while(nodestack.size() > 0) {
        //get current node
        curNode = nodestack.top();
        nodestack.pop();
        if(curNode->num_args == 0) {
            dimOps.push_back((trigenerator *)curNode);
        }
        for(int j = 0; j < curNode->num_args; j++) {
            //create new edge and update nodes
            inputNode = readyNodes.top();
            readyNodes.pop();
            edge = new triedge(curNode, inputNode);
            curNode->myInputs.push_back(edge);
            inputNode->myOutputs.push_back(edge);
        }
        readyNodes.push(curNode);
    }

    //readynodes has one node and its the startnode
    DAGContainer *tree = new DAGContainer(dimOps, readyNodes.top());
    return tree;
}
