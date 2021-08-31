//
//  Board.cpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole):m_nHoles(1){
    if (nHoles > 1)
        m_nHoles = nHoles;
    northBeans = new int[m_nHoles + 1];             // Create arrays of correct size
    southBeans = new int[m_nHoles + 1];
    northBeans[0] = 0;
    southBeans[0] = 0;
    if (nInitialBeansPerHole >= 0) {
        for (int i = 1; i < nHoles+1; i++) {
            southBeans[i] = nInitialBeansPerHole;       // fill them with correct beans
            northBeans[i] = nInitialBeansPerHole;
        }
    }
}

Board::Board(const Board &oldBoard):m_nHoles(oldBoard.m_nHoles){
    northBeans = new int[oldBoard.m_nHoles + 1];
    southBeans = new int[oldBoard.m_nHoles + 1];            // COPY CONSTRUCTOR
    for (int i = 0; i < oldBoard.m_nHoles + 1; i++) {
        northBeans[i] = oldBoard.northBeans[i];
        southBeans[i] = oldBoard.southBeans[i];
    }
}

Board::~Board(){
    delete northBeans;
    delete southBeans;
}

int Board::holes() const{
    return m_nHoles;
}

int Board::beans(Side s, int hole) const{
    if (hole < 0 || hole > m_nHoles){
        return -1;
    }
    if (s == NORTH) {
        return northBeans[hole];
    }
    if (hole == 0) {
        return southBeans[0];
    }
    return southBeans[m_nHoles + 1 - hole];
}

int Board::beansInPlay(Side s) const{
    int sum = 0;
    if (s == NORTH) {                               // Sum over all beans on the side
        for (int i = 1; i < m_nHoles + 1; i++) {
            sum += northBeans[i];
        }
    }else if(s == SOUTH){
        for (int i = 1; i < m_nHoles + 1; i++) {
            sum += southBeans[i];
        }
    }
    return sum;
}

int Board::totalBeans() const{
    return (beansInPlay(NORTH) + beansInPlay(SOUTH) + northBeans[0] + southBeans[0]);
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    if (s == SOUTH) {
        hole = m_nHoles + 1 - hole;
    }
    if (s == SOUTH) {
        if (hole < 1 || hole > m_nHoles || beans(s, m_nHoles + 1 - hole) == 0) {
            return false;
        }
    }else{
        if (hole < 1 || hole > m_nHoles || beans(s, hole) == 0) {
            return false;
        }
    }
    endSide = s;
    int currentBeans;
    if (s == NORTH) {
        currentBeans = beans(s, hole);              // pick up the beans
    }else{
        currentBeans = beans(s, m_nHoles + 1 - hole);              // pick up the beans
    }
    
    if (s == NORTH){
        northBeans[hole] = 0;
    }else{
        southBeans[hole] = 0;
    }
    
    for (; currentBeans > 0; currentBeans--) {          // LOOP WHILE BEANS ARE LEFT
        hole--;
        if (endSide == NORTH) {
            if (hole > 0) {
                northBeans[hole] += 1;
            }else if(hole == 0){                    // protocol for when landing in a pot
                if (s == NORTH) {
                    northBeans[hole] += 1;
                }else if (s == SOUTH){
                    currentBeans++;
                }
                endSide = SOUTH;
                hole = m_nHoles + 1;
            }
        }else if(endSide == SOUTH){              /// ENDSIDE IS SOUTH
            if (hole > 0) {
                southBeans[hole] += 1;
            }else if(hole == 0){
                if (s == SOUTH) {
                    southBeans[hole] += 1;
                }else if (s == NORTH){
                    currentBeans++;
                }
                endSide = NORTH;
                hole = m_nHoles + 1;
            }
        }
    }
    
    if (hole == m_nHoles + 1) {
        endSide = opponent(endSide);
        endHole = 0;
        return true;
    }
    if (endSide == SOUTH) {
        endHole = m_nHoles + 1 - hole;
    }
    endHole = hole;
    return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner){
    if (hole < 1 || hole > m_nHoles) {
        return false;
    }
    if (s == NORTH) {// NORTH
        if (potOwner == NORTH) {
            northBeans[0] += northBeans[hole];  // Put all beans in correct pot
        }else if (potOwner == SOUTH){
            southBeans[0] += northBeans[hole];
        }
        northBeans[hole] = 0;
    }else if(s == SOUTH){// SOUTH
        if (potOwner == NORTH) {
            northBeans[0] += southBeans[m_nHoles + 1 - hole];
        }else if (potOwner == SOUTH){
            southBeans[0] += southBeans[m_nHoles + 1 - hole];
        }
        southBeans[m_nHoles + 1 - hole] = 0;
    }
    return true;
}


bool Board::setBeans(Side s, int hole, int beans){
    if (hole < 1 || hole > m_nHoles) {
        return false;
    }
    if (s == NORTH) {
        if (northBeans[hole] < 0) {
            return false;
        }
        northBeans[hole] = beans;
    }else if (s == SOUTH){
        if (southBeans[hole] < 0) {
            return false;
        }
        if (hole == 0) {
            southBeans[0] = beans;
            return true;
        }
        southBeans[m_nHoles + 1 - hole] = beans;
    }
    return true;
}




