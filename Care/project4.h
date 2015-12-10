#ifndef Care_project4_h
#define Care_project4_h

#include <limits>

struct Cage {
    int index;
    int xCoord;
    int yCoord;
    int wild;

    bool visited;
    double distance;
    int parent;
    
    Cage(int i, int x, int y, int w, bool v): index(i), xCoord(x), yCoord(y), wild(w),
                    visited(v), distance(std::numeric_limits<double>::infinity()) {}
};

double findDistance(int index, int i, std::vector<Cage> &zooCages);


#endif
