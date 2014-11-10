#ifndef __PPIX_H_INCLUDED__
#define __PPIX_H_INCLUDED__

#include "EasyBMP.h"
#include <vector>
#include <string>
#include "dagcontainer.h"

class image_generator {
  private:
    int _width;
    int _height;
    int _framenum;
    int _bitdepth;
  public:
    image_generator(int width, int height);
    void generateFromDAG(DAGContainer *dag, BMP &image, double curT);
}; 

class video_generator {
  private:
    int _width;
    int _height;
    int _framecount;
    int _threads;
    int _bitdepth;
  public:
    video_generator(int width, int height, int framecount, int threads, int bitdepth);
    void generateFromDAG(DAGContainer *dag);
    static void createImageRange(DAGContainer *dag, int width, int height, int starT, int endT, int maxT, int bitdepth);
};

class shader_generator {
  public:
    shader_generator() {}
    std::string generateFromDAG(DAGContainer *dag);
};

class trival {
  public:
    trival(double inr, double ing, double inb);
    double r;
    double g;
    double b;
};
#endif
