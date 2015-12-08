//
//  project4.h
//  Care
//
//  Created by Hannah Wang on 12/7/15.
//  Copyright (c) 2015 Hannah. All rights reserved.
//

#ifndef Care_project4_h
#define Care_project4_h

struct Cage {
    int index;
    int xCoord;
    int yCoord;
    bool wild;
    
    Cage(int i, int x, int y, bool w): index(i), xCoord(x), yCoord(y), wild(w) {}
};


#endif
