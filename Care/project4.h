#ifndef Care_project4_h
#define Care_project4_h

#include <limits>

struct Cage {
    int xCoord;
    int yCoord;
    int wild;
    
    bool visited;
    double distance;
    int parent;
    
    Cage(int x, int y, int w, bool v): xCoord(x), yCoord(y), wild(w),
    visited(v), distance(std::numeric_limits<double>::infinity()) {}
};

struct fastCage {
    int xCoord;
    int yCoord;
    bool visited;
    
    fastCage(int x, int y, bool v): xCoord(x), yCoord(y), visited(v) {}
};

double findDistance(int i, int j, std::vector<Cage> &zooCages);

double simpleDistance(int i, int j, std::vector<fastCage> &simpleCages);

double FASTDistance(std::vector<Cage> &zooCages, std::vector<int> &adjacent,
                    std::vector<double> &distanceMatrix);

double MSTDistance(std::deque<int> &unvisited, std::vector<Cage> &zooCages,
                   std::vector<double> &distanceMatrix);

bool promising (double currentDistance, double &minDistance, std::vector<Cage> &zooCages,
                std::vector<double> &distanceMatrix, std::deque<int> &unvisited,
                std::vector<int> &path);

void gen_perms (std::vector<Cage> &zooCages, std::deque<int> &unvisited, std::vector<int> &path,
                std::vector<int> &adjacent, std::vector<double> &distanceMatrix,
                double &minDistance, double currentDistance);


#endif