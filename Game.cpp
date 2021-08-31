//
//  Game.cpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#include "Game.h"


Game::Game(const Board& b, Player* south, Player* north):m_b(b), m_south(south), m_north(north), turn(SOUTH){}
//// MAKE SURE SOUTH PLAYER ALWAYS MOVES FIRST

void Game::display() const{
    //Line 0 - Whos turn:
    if (turn == SOUTH) {
        cout << "South's Turn:" << endl;
    }else if (turn == NORTH){
        cout << "North's Turn:" << endl;
    }
    // Line 1 - NORTH NAME
    cout << "North - " + m_north->name() << endl;
    // Line 2 - NORTH VALUES
    cout << "             ";
    for (int i = 1; i <= m_b.holes(); i++) {
        cout << " " + to_string(m_b.beans(NORTH, i));
    }
    cout << endl;
    // Line 3 - POT VALUES
    cout << "North Pot: " + to_string(beans(NORTH, 0));
    for (int i = 0; i < m_b.holes(); i++) {
        cout << "   ";
    }
    cout << "South Pot: " + to_string(beans(SOUTH, 0));
    cout << endl;
    // Line 4 - SOUTH VALUES
    cout << "             ";
    for (int i = 1; i <= m_b.holes(); i++) {
        cout << " " + to_string(m_b.beans(SOUTH, i));
    }
    cout << endl;
    // Line 5 - SOUTH NAME
    cout << "South - " + m_south->name() << endl << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    if (m_b.beansInPlay(turn) != 0) {
        over = false;
        return;
    }
    over = true;
    if (m_b.beans(NORTH, 0) > m_b.beans(SOUTH, 0)) {
        hasWinner = true;
        winner = NORTH;
    }else if(m_b.beans(NORTH, 0) < m_b.beans(SOUTH, 0)){
        hasWinner = true;
        winner = SOUTH;
    }else{
        hasWinner = false;
    }
}

bool Game::move(){
    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    if (over == true) {
        return false;
    }
    Side endSide;
    int endHole;
    if (turn == NORTH) {            /// CODE FOR NORTH MOVE
        m_b.sow(NORTH, m_north->chooseMove(m_b, NORTH), endSide, endHole);
        if (endSide == NORTH && endHole == 0) {
            // CODE FOR TAKING ANOTHER TURN
            cout << "Take another turn" << endl << endl;
            if(move())
                return true;
            else
                status(over, hasWinner, winner);
                if (over) {
                    /// WHEN GAME ENDS - SWEEP ALL LEFTOVER BEANS INTO RESPECTIVE POTS
                    for (int i = 1; i <= m_b.holes(); i++) {
                        m_b.moveToPot(NORTH, i, NORTH);
                        m_b.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                return false;
        }else if (turn == NORTH && endSide == NORTH && beans(NORTH, endHole) == 1 && beans(SOUTH, endHole) > 0) {
            //// CODE FOR CAPTURE
            m_b.moveToPot(NORTH, endHole, NORTH);  // MOVE END HOLE
            m_b.moveToPot(SOUTH, endHole, NORTH);// MOVE CAPTURED BEANS
        }
    }else if(turn == SOUTH){        /// CODE FOR SOUTH MOVE
        m_b.sow(SOUTH, m_south->chooseMove(m_b, SOUTH), endSide, endHole);
        if (endSide == SOUTH && endHole == 0) {
            //// CODE FOR MOVE AGAIN
            display();
            cout << "Take another turn" << endl;
            if(move())
                return true;
            else
                status(over, hasWinner, winner);
                if (over) {
                    /// WHEN GAME ENDS - SWEEP ALL LEFTOVER BEANS INTO RESPECTIVE POTS
                    for (int i = 1; i <= m_b.holes(); i++) {
                        m_b.moveToPot(NORTH, i, NORTH);
                        m_b.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                return false;
        }else if(turn == SOUTH && endSide == SOUTH && beans(SOUTH, m_b.holes() + 1 - endHole) == 1 && beans(NORTH, m_b.holes() + 1 - endHole) > 0){
            /// CODE FOR CAPTURE
            m_b.moveToPot(SOUTH, m_b.holes() + 1 - endHole, SOUTH);   // MOVE END HOLE
            m_b.moveToPot(NORTH, m_b.holes() + 1 - endHole, SOUTH); // MOVE CAPTURED BEANS
        }
    }
    turn = opponent(turn);
    
    status(over, hasWinner, winner);
    if (over) {
        /// WHEN GAME ENDS - SWEEP ALL LEFTOVER BEANS INTO RESPECTIVE POTS
        for (int i = 1; i <= m_b.holes(); i++) {
            m_b.moveToPot(NORTH, i, NORTH);
            m_b.moveToPot(SOUTH, i, SOUTH);
        }
    }
    return true;
}

void Game::play(){
    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    display();
    if (m_north->isInteractive() || m_south->isInteractive()) {
        while (!over) {
            move();
            display();
            status(over, hasWinner, winner);
        }
    }else{
        cout << "Press Enter to Start Game";
        cin.ignore(256, '\n');
        cout << endl;
        while (true) {
            if (move()){
                display();
                cout << "Press Enter to Continue";
                cin.ignore(256, '\n');
                cout << endl;
            }else{
                display();
                break;
            }
        }
    }
    status(over, hasWinner, winner);
    

    status(over, hasWinner, winner);
    if (hasWinner) {
        if (winner == NORTH) {
            cout << "THE WINNER IS NORTH" << endl;
        }else{
            cout << "THE WINNER IS SOUTH" << endl;
        }
    }else{
        cout << "THE GAME IS A TIE" << endl;
    }
    cout << "North Score: " << m_b.beans(NORTH, 0) << endl;
    cout << "South Score: " << m_b.beans(SOUTH, 0) << endl;
}

int Game::beans(Side s, int hole) const{
    return m_b.beans(s, hole);
}
