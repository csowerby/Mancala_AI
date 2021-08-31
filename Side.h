//
//  Side.h
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#ifndef Side_h
#define Side_h

#include <climits>
#include <vector>
#include <cassert>


enum Side {NORTH, SOUTH};

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}







#endif /* Side_h */
