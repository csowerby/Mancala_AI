//
//  Player.cpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#include "Player.h"


//////////// PLAYER ///////////////

Player::Player(string name):m_name(name){}

string Player::name() const{
    return m_name; 
}

bool Player::isInteractive() const{
    return false; 
}

Player::~Player(){
    
}


///////////////   HUMANPLAYER ////////

HumanPlayer::HumanPlayer(string name):Player(name){}

bool HumanPlayer::isInteractive() const{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int moveChoice;
    while (true) {
        cout << "Input a move: ";
        cin >> moveChoice;
        if (moveChoice > 0 && moveChoice <= b.holes() && b.beans(s, moveChoice) > 0) {
            break;
        }
    }
    cout << endl; 
    return moveChoice;
}



//////////// BADPLAYER ////////////
BadPlayer::BadPlayer(string name): Player(name){}

int BadPlayer::chooseMove(const Board& b, Side s) const{
    int moveChoice;
    while (true) {
        moveChoice = rand() % b.holes() + 1; // pick a random number between 1 and board.holes()
        if (moveChoice > 0 && moveChoice <= b.holes() && b.beans(s, moveChoice) > 0) {
            break;
        }
    }
    return moveChoice;
}


/////////////// SMART PLAYER /////////
void smartChoice(Side s, Board& b, int &bestHole, int &bestValue, int depth, AlarmClock &ac);


SmartPlayer::SmartPlayer(string name):Player(name){}

int SmartPlayer::chooseMove(const Board& b, Side s) const{
    int bestHole = -1;
    int bestValue = 0;
    AlarmClock ac(4900);
    Board smartChoiceBoard(b);
    smartChoice(s, smartChoiceBoard, bestHole, bestValue, 0, ac);
    if (s == SOUTH) {
        bestHole = b.holes() + 1 - bestHole;
    }
    return bestHole;
}




void SmartPlayer::smartChoice(Side s, Board& b, int &bestHole, int &bestValue, int depth, AlarmClock &ac) const{
    
    bestValue = evaluate(b);
    bestHole = -1;
    
    // CHECK IF GAME IS OVER, RETURN CORRECT VALUE
    if (b.beansInPlay(s) == 0) {
        if (s == NORTH) {
            if (bestValue > 0)
                bestValue = INT_MAX;
            else if(bestValue < 0)
                bestValue = INT_MIN;
        }else if(s == SOUTH){
            if (bestValue > 0)
                bestValue = INT_MIN;
            else if(bestValue < 0)
                bestValue = INT_MAX;
        }
        return;
    }
    

    
    // IF GAME IS NOT OVER
    if(ac.timedOut()){
        return;
    }
    if(depth == 4){///// MAKE SURE THIS IS AN EVEN NUMBER SO BOTH SIDES GET EQUAL PLAY
        return;
    }
    
    if (s == NORTH) {                   // setting best values to worst case scenario
        bestValue = INT_MIN;
    }else if (s == SOUTH){
        bestValue = INT_MAX;
    }
    ///// 1 - DETERMINE LEGAL MOVES  ///////////
    
    vector <int> legalMoves;
    for (int i = 1; i <= b.holes(); i++) {
        if (b.beans(s, i) > 0) {
            legalMoves.push_back(i);
        }
    }
    
    
    /// 2 - LOOP THROUGH LEGAL MOVES
    

    
    for (int i = 0; i < legalMoves.size(); i++) {
        Board newBoard(b);
        Side endSide;
        int endHole;
        int tempHole;
        int tempValue;
        if(s == SOUTH){
            newBoard.sow(s, b.holes() + 1 - legalMoves.at(i), endSide, endHole);
        }else{
            newBoard.sow(s, legalMoves.at(i), endSide, endHole);
        }
        
        
        if(s == SOUTH){
            if (legalMoves.at(i) == b.beans(s, b.holes() + 1 - legalMoves.at(i))) {             // IF A DOUBLE MOVE IS POSSIBLE, call function again with new position (SAME DEPTH)
                smartChoice(s, newBoard, tempHole, tempValue, depth, ac);
            }else{
                smartChoice(opponent(s), newBoard, tempHole, tempValue, depth+1, ac);   // IF NO DOUBLE MOVE IS POSSIBLE, CALL FUNCTION WITH NEW POSITION (INCREASED DEPTH)
            }
        }else{
            if (legalMoves.at(i) == b.beans(s, legalMoves.at(i))) {             // IF A DOUBLE MOVE IS POSSIBLE, call function again with new position (SAME DEPTH)
                smartChoice(s, newBoard, tempHole, tempValue, depth, ac);
            }else{
                smartChoice(opponent(s), newBoard, tempHole, tempValue, depth+1, ac);   // IF NO DOUBLE MOVE IS POSSIBLE, CALL FUNCTION WITH NEW POSITION (INCREASED DEPTH)
            }
        }
        
    
        if (s == NORTH) {
            if (tempValue >= bestValue) {
                bestValue = tempValue; 
                bestHole = legalMoves.at(i);
            }
        }else if(s == SOUTH){
            if(tempValue <= bestValue){
                bestValue = tempValue;
                bestHole = legalMoves.at(i);
            }
        }
        
    }
    return;
}

int SmartPlayer::evaluate(Board &b) const{
    return (b.beans(NORTH, 0) - b.beans(SOUTH, 0));
}

/*

if (Charlie == sad)
{
    code pls work :);
}
*/
