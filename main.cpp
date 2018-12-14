//
//  main.cpp
//  Hangman
//
//  Created by Bailey Capuano on 11/21/18.
//  Copyright © 2018 CapuanoCodes. All rights reserved.
//

#include <iostream>
#include "game.cpp"
#include <stdlib.h>
#include <vector>

using namespace std;

Game *myGame = new Game();

int main(int argc, const char * argv[]) {
    myGame->startGame();
    return 0;
}

