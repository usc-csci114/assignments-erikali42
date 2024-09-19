#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

// initializing colors for letters
string green = "\033[92m";
string yellow = "\033[93m";
string coloroff = "\033[0m";

// if the letter is in the word and in the right place
void rightLetterRightPlace(string& guess, string& actual, vector <bool>& correctPosition){
    for(int i = 0; i < 5; i++){
        if(guess[i] == actual[i]){
            correctPosition[i] = true;
        }
    }
}

// if the letter is present in the word, but not the correct position
void rightLetterWrongPlace(string& guess, string&actual, vector <bool>& correctLetter, vector <bool>& correctPosition){
    vector<bool> letterUsed (5, false);
    for(int i = 0; i < 5; i++){
        if(correctPosition[i]){
            letterUsed[i] = true;
        }
    }
    for(int i = 0; i < 5; i++){
        if(!correctPosition[i]){ // ignore letters in the right place 
            for(int j = 0; j < 5; j++){
                if(guess[i] == actual[j] && !letterUsed[j]){
                    correctLetter[i] = true;
                    letterUsed[j] = true;
                    break;
                }
            }
        }   
    }
}

// checking which letters in word match actual word
string checkGuess(string& guess, string&actual){
    stringstream ss;

    vector<bool> correctLetter(5, false); // stores status of letters that should be yellow
    vector<bool> correctPosition(5, false); // stores status of letters that should be green 

    rightLetterRightPlace(guess, actual, correctPosition);
    rightLetterWrongPlace(guess, actual, correctLetter, correctPosition); 

    for(int i = 0; i < 5; i++){
        if(correctPosition[i]){ // letters in correct place printed in green
            ss << green << guess[i] << coloroff;
        }
        else if (correctLetter[i]){ // letters in word but wrong place printed in yellow
            ss << yellow << guess[i] << coloroff;
        }
        else{ // letters not in word have no color
            ss << guess[i];
        }
    }
    return ss.str(); // return word with colors
}

bool wordInList(string& guess, vector<string>& wordlist){ // checks if the user input word is in the word list/is a valid guess
    for(int i = 0; i < wordlist.size(); i++){
        if(guess == wordlist[i]){
            return true;
        }
    }
    return false;
}

int main(){

    srand(time(0)); // seeding random number generator 
    vector<string> wordlist; 
    string line;
    ifstream file ("wordlist.txt"); // reading in word list

    while(getline(file,line)){
        wordlist.push_back(line); // adding words from wordlist to vector of strings
    }
    file.close(); 

    cout << endl;
    cout << "Welcome to Wordle!" << endl;
    for(int i = 0; i < 6; i++){
        cout << "*****" << endl; // printing initial game setup/board
    }

    string randomWord = wordlist[rand() % wordlist.size()]; // generating random word from list
    int numAttempts = 6;
    bool gameRunning = true;
    vector <string> prevAttempts; // stores the user's previous guesses


    while(gameRunning){
        cout << endl << "Enter your guess:" << endl;
        string input; 
        cin >> input;

        if(input == "quit"){ // end game if user enters "quit"
            cout << "Exiting game." << endl;
            return 0;
        }
        else if(input == "new"){ // start new game and clear previous game if user enters "new"
            randomWord = wordlist[rand() % wordlist.size()];
            numAttempts = 6;
            prevAttempts.clear();
            cout << "Starting new game!" << endl;
            continue;
        }
        else if(input.size() != 5){ // asks for new input if word isn't 5 letters
            cout << "Invalid word length. The word must have 5 letters." << endl;
            continue;
        }
        else if(!wordInList(input, wordlist)){ // asks for new input if the entered word is not in the word list
            cout << "Invalid word. Please try again." << endl;
            continue;
        }
        
        if(input == randomWord){ // if the entire guess matches the actual word
            cout << "Your guess " << green << input << coloroff << " was correct!" << endl;
            cout << "Enter 'quit' to end or 'new' to start a new game" << endl;
            randomWord = wordlist[rand() % wordlist.size()]; // resetting game 
            numAttempts = 6;
            prevAttempts.clear();
            cin >> input;
        }
        else{
            prevAttempts.push_back(checkGuess(input, randomWord)); // adding user's guess to the previous attempts vector
            numAttempts--;
            cout << endl;
            for(int i = 0; i < prevAttempts.size(); i++){ // print out the user's previous inputs
                cout << prevAttempts[i] << endl;
            }
            for(int i = 0; i < numAttempts; i++){ // print board
                cout << "*****" << endl;
            }
            cout << endl;

            if(numAttempts == 0){ // when the user runs out of attempts
                cout << "You lost. The correct word was " << randomWord << endl;
                cout << "Enter 'quit' to end or 'new' to start a new game" << endl;
                randomWord = wordlist[rand() % wordlist.size()]; // resetting game
                numAttempts = 6; 
                prevAttempts.clear();
                cin >> input; 
            }
        }
    }   
    return 0; 
}

