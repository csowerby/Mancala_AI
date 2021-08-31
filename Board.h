//
//  Board.hpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#ifndef Board_h
#define Board_h

#include "Side.h"


class Board{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    Board(const Board &oldBoard);
    ~Board(); 
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
    
private:
    int increment(Side &s, int holes, int inc); 
    int m_nHoles;
    int *northBeans;
    int *southBeans;
};



#endif /* Board_h */
