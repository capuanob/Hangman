//
//  NPC.cpp
//  Hangman
//
//  Created by Bailey Capuano on 11/21/18.
//  Copyright © 2018 CapuanoCodes. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

/// For my implementation of hangman, the NPC will get 8 wrong answers before losing.

class NPC {
    
private: string myGuess;
private: bool hasWon = false;
private: vector<char> availableGuesses;
private: vector<int> vowelIndices;
private: int wordLength = 0; 
private: vector<string> knowledgeBase;
public: int incorrectGuesses = 0;
public: int guessCount = 0;
    
//Used solely for testing purposes, view state of a given vector<string>.
private: void printVector(vector<int> vec) {
    for (int i = 0; i < int(vec.size()); i++) {
        cout << vec[i] << endl;
    }
}

//Populates the availableGuesses vector with the alphabet. 
private: void fillAlphabet() {
    for (int ascii = 97; ascii < 123; ascii++) {
        const char asLetter = char(ascii);
        availableGuesses.push_back(asLetter);
    }
    
    //Load the vowels, to be used for determining which value to guess first.
    vowelIndices.push_back(0);
    vowelIndices.push_back(4);
    vowelIndices.push_back(8);
    vowelIndices.push_back(14);
    vowelIndices.push_back(20);
}
    
//Fills guess with underscores according to wordLength.
private: void populateGuess(char l, vector<int> indices) {
    
    if (indices.empty()) {
        for (int i = 0; i < wordLength; i++) {
            myGuess += '_';
        }
    } else {
        for (int i = 0; i < indices.size(); i++) {
            myGuess[indices[i] - 1] = l;
        }
    }
}
    
//Tells user the rules and sets wordLength.
public: void introduceSelf() {
    cout << "Welcome to Hangman! Let's cover the rules..." << endl;
    cout << "\t 1. Your word has to be between 1-9 characters in length." << endl;
    cout << "\t 2. Your word cannot be a proper noun (think no capitalized words)" << endl;
    cout << "\nWhat's the length of your word?" << endl;
	while (!(cin >> wordLength) || wordLength > 9 || wordLength < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.  Try again: ";
	}
	cout << endl; 
    knowledgeBase = loadKnowledge();
    fillAlphabet();
}

//Returns a vector of all possible guesses.
private: vector<string> loadKnowledge() {
    ifstream file;
    vector<string> options;
    file.open("dictionary.txt");
    
    if (file.is_open()) {
       
        string value = "empty";
        
        while (value != "") {
            getline(file, value, '\n');
            
            if (value.length() == wordLength) {
                options.push_back(value);
            } else if (options.size() != 0) {
                break;
            }
        }
        
        file.close();
    } 
    return options;
}

//Remove words that don't have letters in the corresponding positions of myGuess.
private: void updateKnowledgeBase(vector<int> indices) {
    vector<string> safeWords;
    for (int i = 0; i < knowledgeBase.size(); i++) {
        safeWords.push_back(knowledgeBase[i]);
        for (int j = 0; j < indices.size(); j++) {
            
            if (myGuess[indices[j]-1] == '_') {
                continue;
            }
            
            if (knowledgeBase[i][indices[j]-1] != myGuess[indices[j]-1]) {
                safeWords.erase(safeWords.end() - 1);
                break;
            }
        }
    }
    knowledgeBase = safeWords;
}

private: void removeUnrulyWords(char l) {
    vector<string> temp;
    for (int i = 0; i < knowledgeBase.size(); i++) {
        for (int j = 0; j < knowledgeBase[i].length(); j++) {
            if (knowledgeBase[i][j] == l) {
                break;
            }
            if (j == knowledgeBase[i].length() - 1) {
                temp.push_back(knowledgeBase[i]);
            }
        }
        
    }
    knowledgeBase = temp;
}
    
//Used to parse indices of guessed letter (e.g. 1 or 1, 5)
private: vector<int> parseIndexInputs(string input) {
    vector<int> temp;
    for (int i = 0; i < input.length(); i++) {
        if (isdigit(input[i]) && int(input[i] - 48) > 0 && int(input[i] - 48) <= wordLength) {
            temp.push_back(int(input[i]) - 48);
        }
    }
    return temp;
}
 
//Used to ascertain what the user's word of choice is.
public: string guess() {

    bool vowelFound = false;
	int largestIndex = 0;
	vector<int> indicesPresent;
	string input;
	vector<int> letterPresenceAtIndex;
	int largestValue = 0;
	bool doesContain = false; 

	//End case, solution found
    if (knowledgeBase.size() == 1) {
        return knowledgeBase[0];
    }
    
	//Initially populates myGuess with '_' for each index.
    if (guessCount == 0) {
        populateGuess(0, indicesPresent);
    }
    
    guessCount++;
    
	//Populate letterPresenceAtIndex with the numerical occurence of each letter at the index to be guessed. 
    for (int i = 0; i < 26; i++) {
        letterPresenceAtIndex.push_back(0);
    }
    
    for (int i = 0; i < knowledgeBase.size(); i++) {
		for (int j = 0; j < knowledgeBase[i].size(); j++) {
			letterPresenceAtIndex[(knowledgeBase[i][j]) - 97] += 1;
		}
    }
    
    //Get index of largest element in letterPresenceAtIndex and set it to indexToGuess
    //TODO: Ask existing vowels until one is found.

	for (int i = 0; i < letterPresenceAtIndex.size(); i++) {
		if (availableGuesses[i] == '*') { continue; }

		if (find(vowelIndices.begin(), vowelIndices.end(), i) != vowelIndices.end() && !vowelFound) {
			if (letterPresenceAtIndex[i] > largestValue) {
				largestIndex = i;
				largestValue = letterPresenceAtIndex[i];
			}
		}
		else {
			if (letterPresenceAtIndex[i] > largestValue) {
				largestIndex = i;
				largestValue = letterPresenceAtIndex[i];
			}
		}
	}
	
	cout << "Does your word contain " << availableGuesses[largestIndex] << "? ('Y' or 'N')" << endl;
	
	char response;
	cin >> response;
	while (response != 'Y' && response != 'N') {
		cout << "Invalid input.  Try again: ";
		cin >> response; 
	}

	if (response == 'Y') {
		if (find(vowelIndices.begin(), vowelIndices.end(), largestIndex) != vowelIndices.end()) {
			vowelFound = true; 
		}
		doesContain = true;
	}

    // Updates myGuess and deletes non-abiding strings from the knowledgeBase.
    if (doesContain) {
        cout << "Please provide the position where the letter appears." << endl;
        cout << "E.g. 1 or 1, 5" << endl;
        cin.ignore();
        getline(cin, input);
        indicesPresent = parseIndexInputs(input);
        
        populateGuess(availableGuesses[largestIndex], indicesPresent);
        updateKnowledgeBase(indicesPresent);
        
    } else {
        removeUnrulyWords(availableGuesses[largestIndex]);
        incorrectGuesses++;
    }
    
	availableGuesses[largestIndex] = '*';
    cout << endl << "So far we have " << myGuess << endl;

    for (int i = 0; i < myGuess.size(); i++) {
        if (myGuess[i] == '_') {
            return "";
        }
    }
    return myGuess;
}

};

