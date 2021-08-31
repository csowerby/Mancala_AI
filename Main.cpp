#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

int main(){
    Board b(4, 2);
    SmartPlayer p1("Charlie");
    BadPlayer p2("CS32");
    Game g(b, &p1, &p2);
    
    g.play();
}
