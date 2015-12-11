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

double MSTDistance(std::vector<Cage> &zooCages);

void gen_perms (std::vector<Cage> &zooCages, std::deque<int> &unvisited,
                std::vector<int> &path, std::vector<int> &adjacent,
                double MST, double &minDistance, double &currentDistance);

bool promising (double MST, double &currentDistance, int xCoord1, int yCoord1,
                int xCoord2, int yCoord2);
#endif
