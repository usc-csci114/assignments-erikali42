#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <string> 
#include <map>

using namespace std;

// function to read csv data
vector<vector<string> > readCSV(const string& filename) {
    vector<vector<string> > data;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}


int main(){
// data table
    string filename = "bug-attraction.csv"; 
    vector<vector<string> > data = readCSV(filename);

    map<string, vector<int> > mapBugType; // string = name of bug, vector of size 6 = counts for each type of light. 0 = A, 1 = B, 2 = C, 3 = LED, 4 = CFL, 5 = no light
    vector<string> lightTypes = {"A", "B", "C", "LED", "CFL", "No Light"};
    for(int i = 1; i < data.size(); ++i){ // going thru each block 
        for(int j = 18; j < 32; ++j){ 
            string bugType = data[0][j]; // bug type in top row
            int lightIndex = (i-1)%6; // index of light type
            int countsPerLight = stoi(data[i][j]); // count value 
            if (mapBugType.find(bugType) == mapBugType.end()) { // does bug type vector exist? 
                mapBugType[bugType] = vector<int>(6, 0); // initialize with zeros
            }
            mapBugType[bugType][lightIndex] += countsPerLight; // increment count for light type
        }
    }
    
    // displaying the table
    cout << left << setw(20) << "Species" << setw(15) << "Most Attractive Light" << endl;
    cout << string(35, '-') << endl;

    for(const auto&[bugType, lightCounts] : mapBugType){
        int maxIndex = -1;
        int maxCount = -1;
    // finding the light type with the most bugs
        for(int i = 0; i < 5; ++i){
            if(lightCounts[i] > maxCount){
                maxCount = lightCounts[i];
                maxIndex = i;
            }
        }
        string lightType = lightTypes[maxIndex];
        cout << left << setw(20) << bugType << setw(15) << lightType << endl;

    }
    return 0;
}



