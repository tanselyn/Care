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
    
    string input;
    
    double totalDistance = 0;
    int index = 0;
    int pos = 0;
    int visitCages = 0;
    char type;
    char mode = '?';
    
    os << fixed << setprecision(2);
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
        
        index = 0;
        while (visitCages < (int)zooCages.size()) {
    	    double minDistance = numeric_limits<double>::infinity();
            int minIndex = 0;
            for (int i = 1; i < (int)zooCages.size(); ++i) {
                if (zooCages[i].visited == false) {
                    if ((zooCages[i].wild == 0 && zooCages[index].wild == 1)||
                        (zooCages[i].wild == 0 && zooCages[index].wild == 0) ||
                        zooCages[i].wild == 1 ||
                        (zooCages[i].wild == 2 && zooCages[index].wild == 1) ||
                        (zooCages[i].wild == 2 && zooCages[index].wild == 2)) {
                        
                        double x = zooCages[index].xCoord - zooCages[i].xCoord;
                        double y = zooCages[index].yCoord - zooCages[i].yCoord;
                        double distance = sqrt((x * x) + (y * y));
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
            if (index != 0) {
                os << min(index,zooCages[index].parent) << " "
                    << max(index,zooCages[index].parent) << '\n';
            }
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
    }
    if (mode == 'A') {
    }
    
    cout << os.str();
    return 0;
}