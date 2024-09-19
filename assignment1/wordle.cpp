#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

string green = "\033[92m";
string yellow = "\033[93m";
string coloroff = "\033[0m";

void rightLetterRightPlace(string& guess, string& actual, vector <bool>& correctPosition){
    for(int i = 0; i < 5; i++){
        if(guess[i] == actual[i]){
            correctPosition[i] = true;
        }
    }
}

void rightLetterWrongPlace(string& guess, string&actual, vector <bool>& correctLetter, vector <bool>& correctPosition){
    vector<bool> letterUsed (5, false);
    for(int i = 0; i < 5; i++){
        if(correctPosition[i]){
            letterUsed[i] = true;
        }
    }
    for(int i = 0; i < 5; i++){
        if(!correctPosition[i]){
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

string checkGuess(string& guess, string&actual){
    stringstream ss;

    vector<bool> correctLetter(5, false);
    vector<bool> correctPosition(5, false);

    rightLetterRightPlace(guess, actual, correctPosition);
    rightLetterWrongPlace(guess, actual, correctLetter, correctPosition); 

    for(int i = 0; i < 5; i++){
        if(correctPosition[i]){
            ss << green << guess[i] << coloroff;
        }
        else if (correctLetter[i]){
            ss << yellow << guess[i] << coloroff;
        }
        else{
            ss << guess[i];
        }
    }
    return ss.str();
}

bool wordInList(string& guess, vector<string>& wordlist){
    for(int i = 0; i < wordlist.size(); i++){
        if(guess == wordlist[i]){
            return true;
        }
    }
    return false;
}

int main(){

    srand(time(0));
    vector<string> wordlist; 
    string line;
    ifstream file ("wordlist.txt");

    while(getline(file,line)){
        wordlist.push_back(line);
    }
    file.close();
    cout << endl;
    cout << "Welcome to Wordle!" << endl;
    for(int i = 0; i < 6; i++){
        cout << "*****" << endl;
    }

    string randomWord = wordlist[rand() % wordlist.size()];
    int numAttempts = 6;
    bool gameRunning = true;
    vector <string> prevAttempts; 


    while(gameRunning){
        cout << endl << "Enter your guess:" << endl;
        string input; 
        cin >> input;

        if(input == "quit"){
            cout << "Exiting game." << endl;
            return 0;
        }
        else if(input == "new"){
            randomWord = wordlist[rand() % wordlist.size()];
            numAttempts = 6;
            prevAttempts.clear();
            cout << "Starting new game!" << endl;
            continue;
        }
        else if(input.size() != 5){
            cout << "Invalid word length. The word must have 5 letters." << endl;
            continue;
        }
        else if(!wordInList(input, wordlist)){
            cout << "Invalid word. Please try again." << endl;
            continue;
        }
        
        if(input == randomWord){
            cout << "Your guess " << green << input << coloroff << " was correct!" << endl;
            cout << "Enter 'quit' to end or 'new' to start a new game" << endl;
            randomWord = wordlist[rand() % wordlist.size()];
            numAttempts = 6;
            prevAttempts.clear();
            cin >> input;
        }
        else{
            prevAttempts.push_back(checkGuess(input, randomWord));
            numAttempts--;
            cout << endl;
            for(int i = 0; i < prevAttempts.size(); i++){
                cout << prevAttempts[i] << endl;
            }
            for(int i = 0; i < numAttempts; i++){
                cout << "*****" << endl;
            }
            cout << endl;

            if(numAttempts == 0){
                cout << "You lost. The correct word was " << randomWord << endl;
                cout << "Enter 'quit' to end or 'new' to start a new game" << endl;
                randomWord = wordlist[rand() % wordlist.size()];
                numAttempts = 6; 
                prevAttempts.clear();
                cin >> input; 
            }
        }
    }   
    return 0; 
}

