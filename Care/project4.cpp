#include <iostream>
#include <iomanip>
#include <getopt.h>
#include <vector>
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

int main(int argc, char * argv[])
{
    ios_base::sync_with_stdio(false);
    
    ostringstream os;
    
    vector<Cage> zooCages;
    
    pair<int,double> minDistance(0,numeric_limits<double>::infinity());
    string input;
    
    double totalDistance = 0;
    int index = 0;
    int pos = 0;
    int visitCages = 0;
    char type;
    char mode;
    
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
        if (insert.xCoord == 0 || insert.yCoord == 0) {
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
        
        while (visitCages < zooCages.size() - 1) {
            for (int i = 0; i < (int)zooCages.size(); ++i) {
                if (zooCages[i].visited == true) {
                    for (int j = 1; j < (int)zooCages.size(); ++j) {
                        if (zooCages[j].visited == false) {
                            if (zooCages[j].wild == 0 || (zooCages[j].wild == 2 &&
                                zooCages[i].wild == 1)) {
                                double distance = sqrt((zooCages[i].xCoord - zooCages[j].xCoord) *
                                                       (zooCages[i].xCoord - zooCages[j].xCoord) +
                                                       (zooCages[i].yCoord - zooCages[j].yCoord) *
                                                       (zooCages[i].yCoord - zooCages[j].yCoord));
                                if (distance < zooCages[j].distance) {
                                    zooCages[j].distance = distance;
                                    zooCages[j].parent = i;
                                }
                                if (zooCages[j].distance < minDistance.second) {
                                    minDistance.first = j;
                                }
                            }
                        }
                    }
                }
            }
            zooCages[minDistance.first].visited = true;
            ++visitCages;
        }
        for (int i = 0; i < (int)zooCages.size(); ++i) {
            totalDistance += zooCages[i].distance;
        }
        os << fixed << setprecision(2) << totalDistance << '\n';
        for (int i = 0; i < (int)zooCages.size(); ++i) {
            if (zooCages[i].visited == false) {
                cerr << "Cannot construct MST" << '\n';
                exit(1);
            }
            os << min(i,zooCages[i].parent) << " " << max(i,zooCages[i].parent) << '\n';
        }
        
    }
    if (mode == 'P') {
        cout << "OPTTSP" << endl;
    }
    if (mode == 'A') {
        cout << "FASTTSP" << endl;
    }
    
    cout << os.str();
    return 0;
}
