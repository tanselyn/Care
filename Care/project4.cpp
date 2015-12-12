#include <iostream>
#include <iomanip>
#include <algorithm>
#include <getopt.h>
#include <vector>
#include <deque>
#include <string>
#include <cmath>
#include <sstream>
#include "project4.h"

using namespace std;

static struct option longopts[] = {
    {"mode", required_argument, nullptr, 'm'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
};

double findDistance(int i, int j, vector<Cage> &zooCages) {
    double x = zooCages[i].xCoord - zooCages[j].xCoord;
    double y = zooCages[i].yCoord - zooCages[j].yCoord;
    return sqrt((x * x) + (y * y));
}

double FASTDistance(vector<Cage> &zooCages, vector<int> &adjacent,
                    vector<double> &distanceMatrix) {
    double totalDistance = 0;
    zooCages.front().distance = 0;
    zooCages.front().visited = true;
    zooCages.front().parent = 0;
    
    int visitCages = 0;
    int index = 0;
    while (visitCages < (int)zooCages.size() - 1) {
    	double minDistance = numeric_limits<double>::infinity();
        int minIndex = 0;
        for (int i = 1; i < (int)zooCages.size(); ++i) {
            if (zooCages[i].visited == false) {
                double distance = distanceMatrix[(index * zooCages.size())
                                                 + i];
                if (distance < minDistance) {
                    minIndex = i;
                    minDistance = distance;
                }
            }
        }
        zooCages[minIndex].visited = true;
        index = minIndex;
        adjacent.push_back(minIndex);
        ++visitCages;
    }
    adjacent.push_back(0);
    
    for (int i = 0; i < (int)adjacent.size() - 3; ++i) {
        for (int j = i + 2; j < (int)adjacent.size() - 1; ++j) {
            double change = distanceMatrix[(adjacent[i] * zooCages.size()) +
                                           adjacent[j]] +
            distanceMatrix[(adjacent[i + 1] * zooCages.size()) +
                           adjacent[j + 1]] -
            distanceMatrix[(adjacent[i] * zooCages.size()) +
                           adjacent[i + 1]] -
            distanceMatrix[(adjacent[j] * zooCages.size()) +
                           adjacent[j + 1]];
            if (change < 0) {
                reverse(adjacent.begin() + i + 1, adjacent.begin() + j + 1);
            }
        }
    }
    for (int i = 0; i < (int)zooCages.size() - 1; ++i) {
        totalDistance += distanceMatrix[(adjacent[i] * zooCages.size()) +
                                        adjacent[i + 1]];
    }
    totalDistance += distanceMatrix[adjacent[adjacent.size() - 2]];
    return totalDistance;
}

double MSTDistance(deque<int> &unvisited, vector<Cage> &zooCages, vector<double> &distanceMatrix) {
    zooCages[unvisited.front()].distance = 0;
    zooCages[unvisited.front()].visited = true;
    zooCages[unvisited.front()].parent = unvisited.front();
    
    double totalDistance  = 0 ;
    int index = unvisited.front();
    int visitCages = 0;
    while (visitCages < (int)unvisited.size() - 1) {
        double minDistance = numeric_limits<double>::infinity();
        int minIndex = 0;
        for (int i = 1; i < (int)unvisited.size(); ++i) {
            if (zooCages[unvisited[i]].visited == false) {
                double distance = distanceMatrix[(index * zooCages.size()) + unvisited[i]];
                if (distance < zooCages[unvisited[i]].distance) {
                    zooCages[unvisited[i]].distance = distance;
                    zooCages[unvisited[i]].parent = index;
                }
                if (zooCages[unvisited[i]].distance < minDistance) {
                    minIndex = unvisited[i];
                    minDistance = zooCages[unvisited[i]].distance;
                }
            }
        }
        index = minIndex;
        zooCages[index].visited = true;
        totalDistance += minDistance;
        //cerr << minDistance << " " << totalDistance << endl;
        ++visitCages;
    }
    for (int i = 0; i < (int)zooCages.size(); ++i) {
        zooCages[i]. distance = numeric_limits<double>::infinity();
        zooCages[i].visited = false;
    }
    return totalDistance;
    
}

bool promising (double currentDistance, double &minDistance, vector<Cage> &zooCages,
                vector<double> &distanceMatrix, deque<int> &unvisited, vector<int> &path) {
    //cerr << currentDistance << endl;
    currentDistance += MSTDistance(unvisited, zooCages, distanceMatrix);
    //cerr << currentDistance << endl;
    double minConnection1 = numeric_limits<double>::infinity();
    double minConnection2 = numeric_limits<double>::infinity();
    for (int i = 0; i < (int)unvisited.size(); ++i) {
        for (int j = 0; j < (int)path.size(); ++j) {
            if (distanceMatrix[(unvisited[i] * zooCages.size()) + path[j]] <
                minConnection1) {
                minConnection1 = distanceMatrix[(unvisited[i] * zooCages.size()) + path[j]];
            }
            else if (distanceMatrix[(unvisited[i] * zooCages.size()) + path[j]] <
                     minConnection2) {
                minConnection2 = distanceMatrix[(unvisited[i] * zooCages.size()) + path[j]];
            }
        }
    }
    currentDistance += minConnection1;
    currentDistance += minConnection2;
    if (currentDistance < minDistance) return true;
    else return false;
}

void gen_perms (vector<Cage> &zooCages, deque<int> &unvisited, vector<int> &path,
                vector<int> &adjacent, vector<double> &distanceMatrix,
                double &minDistance, double currentDistance) {
    currentDistance += distanceMatrix[(path.back() * zooCages.size()) +
                                      path[path.size() - 2]];
    //cerr << "hello" << endl;
    //cerr <<  minDistance << endl;
    if (unvisited.empty()) {
        currentDistance += distanceMatrix[path[path.size() - 2]];
        
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            adjacent = path;
            
        }
        return;
    }
    //cerr << unvisited.size() << endl;
    if (!promising(currentDistance, minDistance, zooCages, distanceMatrix,
                   unvisited, path)) return;
    for (int i = 0; i < (int)unvisited.size(); i++) {
        path.push_back(unvisited.front());
        unvisited.pop_front();
        gen_perms(zooCages, unvisited, path, adjacent, distanceMatrix,
                  minDistance, currentDistance);
        unvisited.push_back(path.back());
        path.pop_back();
    }
}

int main(int argc, char * argv[])
{
    ios_base::sync_with_stdio(false);
    
    ostringstream os;
    
    vector<Cage> zooCages;
    
    string input;
    double totalDistance = 0;
    int visitCages = 0;
    int index = 0;
    int pos = 0;
    char type;
    char mode = '?';
    
    while ((type = getopt_long(argc, argv, "m:h", longopts, &index)) != -1) {
        switch (type) {
            case 'm':
                mode = optarg[1];
                break;
            case 'h':
                cout << "Need some help? This program finds paths and stuff. " << '\n';
                exit(0);
                break;
        }
    }
    if (mode != 'S' && mode != 'P' && mode != 'A') {
        cerr << "Invalid mode type" << '\n';
        exit(1);
    }
    
    getline(cin,input);
    index = stoi(input);
    while (index > 0) {
        getline(cin,input);
        pos = (int)input.find(' ',0);
        Cage insert((int)zooCages.size(),stoi(input.substr(0,pos)),stoi(input.substr(pos)),
                    0,false);
        if ((insert.xCoord == 0 && insert.yCoord <= 0) ||
            (insert.yCoord == 0 && insert.xCoord <= 0)) {
            insert.wild = 1;
        }
        else if (insert.xCoord < 0 && insert.yCoord < 0) {
            insert.wild = 2;
        }
        zooCages.push_back(insert);
        --index;
    }
    
    if (mode == 'S') {
        zooCages.front().distance = 0;
        zooCages.front().visited = true;
        zooCages.front().parent = 0;
        
        index = 0;
        while (visitCages < (int)zooCages.size() - 1) {
    	    double minDistance = numeric_limits<double>::infinity();
            int minIndex = 0;
            for (int i = 1; i < (int)zooCages.size(); ++i) {
                if (zooCages[i].visited == false) {
                    if ((zooCages[i].wild == 0 && zooCages[index].wild == 1)||
                        (zooCages[i].wild == 0 && zooCages[index].wild == 0) ||
                        zooCages[i].wild == 1 ||
                        (zooCages[i].wild == 2 && zooCages[index].wild == 1) ||
                        (zooCages[i].wild == 2 && zooCages[index].wild == 2)) {
                        
                        double distance = findDistance(index, i, zooCages);
                        if (distance < zooCages[i].distance) {
                            zooCages[i].distance = distance;
                            zooCages[i].parent = index;
                        }
                    }
                    if (zooCages[i].distance < minDistance) {
                        minIndex = i;
                        minDistance = zooCages[i].distance;
                    }
                }
            }
            index = minIndex;
    	    zooCages[index].visited = true;
            os << min(index,zooCages[index].parent) << " "
            << max(index,zooCages[index].parent) << '\n';
            ++visitCages;
        }
        for (int i = 1; i < (int)zooCages.size(); ++i) {
            if (zooCages[i].visited == false) {
                cerr << "Cannot construct MST" << '\n';
                exit(1);
            }
            totalDistance += zooCages[i].distance;
        }
        cout << fixed << setprecision(2) << totalDistance << '\n';
    }
    if (mode == 'P') {
        vector<int> adjacent;
        deque<int> unvisited;
        vector<int> path;
        vector<double> distanceMatrix;
        double minDistance = 0;
        double currentDistance = 0;
        double totalDistance = 0;
        
        adjacent.push_back(0);
        path.push_back(0);
        
        for (int i = 0; i < (int)zooCages.size(); ++i) {
            for (int j = 0; j < (int)zooCages.size(); ++j) {
                distanceMatrix.push_back(findDistance(i, j, zooCages));
            }
        }
        
        minDistance = FASTDistance(zooCages, adjacent, distanceMatrix);
        
        zooCages[0].distance = numeric_limits<double>::infinity();
        zooCages[0].visited = false;
        for (int i = 1; i < (int)zooCages.size(); ++i) {
            unvisited.push_back(i);
            zooCages[i].distance = numeric_limits<double>::infinity();
            zooCages[i].visited = false;
        }
        
        gen_perms(zooCages, unvisited, path, adjacent, distanceMatrix,
                  minDistance, currentDistance);
        adjacent.push_back(0);
        for (int i = 0; i < (int)adjacent.size() - 1; ++i) {
            os << adjacent[i] << " ";
            totalDistance += distanceMatrix[(adjacent[i] * zooCages.size()) +
                                            adjacent[i + 1]];
        }
        os << '\n';
        
        cout << fixed << setprecision(2) << totalDistance << '\n';
    }
    if (mode == 'A') {
        vector<int> adjacent;
        zooCages.front().distance = 0;
        zooCages.front().visited = true;
        zooCages.front().parent = 0;
        
        index = 0;
        adjacent.push_back(0);
        while (visitCages < (int)zooCages.size() - 1) {
    	    double minDistance = numeric_limits<double>::infinity();
            int minIndex = 0;
            for (int i = 1; i < (int)zooCages.size(); ++i) {
                if (zooCages[i].visited == false) {
                    double distance = findDistance(index, i, zooCages);
                    if (distance < minDistance) {
                        minIndex = i;
                        minDistance = distance;
                    }
                }
            }
    	    zooCages[minIndex].visited = true;
            index = minIndex;
            adjacent.push_back(minIndex);
            
            ++visitCages;
        }
        adjacent.push_back(0);
        
        for (int i = 0; i < (int)adjacent.size() - 3; ++i) {
            double temp = findDistance(adjacent[i], adjacent[i + 1], zooCages);
            for (int j = i + 2; j < (int)adjacent.size() - 1; ++j) {
                double change = (findDistance(adjacent[i], adjacent[j], zooCages)) +
                (findDistance(adjacent[i + 1], adjacent[j + 1], zooCages)) -
                (temp) - (findDistance(adjacent[j], adjacent[j + 1], zooCages));
                
                if (change < 0) {
                    reverse(adjacent.begin() + i + 1, adjacent.begin() + j + 1);
                    temp = findDistance(adjacent[i], adjacent[i + 1], zooCages);
                }
            }
        }
        
        for (int i = 0; i < (int)zooCages.size() - 1; ++i) {
            totalDistance += findDistance(adjacent[i],adjacent[i + 1], zooCages);
            os << adjacent[i] << " ";
        }
        os << adjacent[adjacent.size() - 2] << '\n';
        totalDistance += findDistance(0, adjacent[adjacent.size() - 2], zooCages);
        
        cout << fixed << setprecision(2) << totalDistance << '\n';
    }
    
    cout << os.str();
    return 0;
}