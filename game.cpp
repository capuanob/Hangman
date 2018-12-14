//
//  game.cpp
//  Hangman
//
//  Created by Bailey Capuano on 11/21/18.
//  Copyright © 2018 CapuanoCodes. All rights reserved.
//

#include "NPC.cpp"
#include <iostream>
using namespace std;

class Game {
    
private: bool isActive;
private: NPC *computer = new NPC();
public: string answer = "";
    
public: void startGame() {
    computer->introduceSelf();
    
    while (answer == "") {
        answer = computer->guess();
        
        if (computer->incorrectGuesses >= 8 ) {
            answer = "lost";
        }
    }
    
    if (answer != "lost") {
        cout << "Your word is: " << answer << endl;
    } else {
        cout << endl << endl << "I've been hung, you win!" << endl << endl;
    }
}
    
};
