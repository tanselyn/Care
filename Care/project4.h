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

double MSTDistance(std::deque<int> &unvisited, std::vector<Cage> zooCages);

void gen_perms (std::vector<Cage> &zooCages, std::deque<int> &unvisited,
                std::vector<int> &path, std::vector<int> &adjacent,
                double &minDistance, double &currentDistance);

bool promising (double currentDistance, double &minDistance, std::vector<Cage> zooCages,
                std::deque<int> &unvisited, int index);
#endif
