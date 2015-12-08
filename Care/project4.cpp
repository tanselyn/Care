//
//  main.cpp
//  Care
//
//  Created by Hannah Wang on 12/7/15.
//  Copyright (c) 2015 Hannah. All rights reserved.
//

#include <iostream>
#include <getopt.h>
#include <vector>
#inclue <string>
#include <sstream>
#include "project4.h"

using namespace std;

static struct option longopts[] = {
    {"mode", required_argument, nullptr, 'm'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
};

int main(int argc, const char * argv[])
{
    ios_base::sync_with_stdio(false);
    
    ostringstream os;

    vector<Cage> zooCages;
    
    string input;
    
    int index = 0;
    int pos = 0;
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

    cin >> index;
    while (getline(cin,input)) {
        pos = (int)input.find(' ',0);
        Cage insert((int)zooCages.size(),stoi(input.substr(0,pos)),stoi(input.substr(pos)),false);
        if (insert.xCoord < 0 && insert.yCoord < 0) {
            insert.wild = true;
        }
        zooCages.push_back(insert);
    }
    
    if (mode == 'S') {
        
    }
    if (mode == 'P') {
        
    }
    if (mode == 'A') {
        
    }
    
    return 0;
}

