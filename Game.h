//
//  Game.hpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "Board.h"
#include "Player.h"

class Game{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
private:
    Board m_b;
    Player* m_south;
    Player* m_north;
    Side turn;
};

#endif /* Game_h*/
