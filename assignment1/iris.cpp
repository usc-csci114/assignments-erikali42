#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector> 
#include <fstream>
#include <sstream>

using namespace std; 

class Info{ // a class that has all the statistical calculations for our values
    public: 
        vector<double> values;
        void addValue(double value){
            values.push_back(value);
        }
        double min() const{
            return *min_element(values.begin(), values.end());
        }
        double max() const{
            return *max_element(values.begin(), values.end());
        }
        double mean() const{
            double sum = 0.0;
            for(int i = 0; i < values.size(); i++){
                sum += values[i];
            }
            return sum/values.size();
        }
        double stdev() const{
            double m = mean();
            double sqSum = 0.0;
            for(int i = 0; i < values.size(); i++){
                sqSum += (values[i] - m) * (values[i] - m);
            }
            return sqrt(sqSum/values.size());
        }
};

// speciesInfo is a 2D vector, where each Info object holds stats for a species
// speciesNames contains the names of all the iris species

void printTable(const vector<vector<Info> >& speciesInfo, const vector<string>& speciesNames){
    cout << fixed << setprecision(2); // 2 decimal points
    cout << setw(60) << "Iris Data" << setw(50) << endl; // prints table title
    cout << string(120, '-') << endl;
    cout << setw(15) << "Species Name" << setw(20) << "Sepal Length" << setw(25) << "Sepal Width" << setw(25) << "Petal Length" << setw(25) << "Petal Width" << endl;
    cout << string(120, '-') << endl;
    for(int i = 0; i < speciesNames.size(); i++){ // iterating through species to print the 4 species names
        cout << setw(15) << speciesNames[i] << setw(5);
        for(int j = 0; j < 4; j++){ // for each species, printing out the info
            const Info& info = speciesInfo[i][j];
            cout << "|" << info.min() << "," << info.max() << "," << info.mean() << "," << info.stdev() << "|" << setw(5);

        }
        cout << endl;
        cout << string(120, '-') << endl;
    }
}

int main(){
    ifstream file ("iris.txt"); // opening file
    if(!file.is_open()){ // checking if file is successfully opened
        cout << "file couldn't be opened!" << endl;
        return 1;
    }

    vector<vector<Info> > speciesInfo(3, vector<Info>(4)); // 2D vector where there are 3 species and 4 pieces of info for each
    vector<string> speciesNames; // stores names of the iris species
    speciesNames.push_back("Setosa");
    speciesNames.push_back("Versicolor");
    speciesNames.push_back("Virginica");

    string line;
    while(getline(file,line)){ // reading each line of the file
        stringstream ss(line); // make a stringstream for each line
        string value;
        double sepalLength, sepalWidth, petalLength, petalWidth;
        string species;

        getline(ss, value, ','); // extract substring from stringstream ss until the delimiter, a comma ",", and stores it in value
        sepalLength = stod(value); // set the sepal length to the extracted value, converting the string to a double

        getline(ss, value, ',');
        sepalWidth = stod(value);

        getline(ss, value, ',');
        petalLength = stod(value);
        
        getline(ss, value, ',');
        petalWidth = stod(value);

        getline(ss, species); // takes the last part of the line, or the species name, and sets it to the species name

        int speciesIndex = -1; // setting the index of each species
        if(species == "Iris-setosa"){
            speciesIndex = 0;
        }
        else if(species == "Iris-versicolor"){
            speciesIndex = 1;
        }
        else if(species == "Iris-virginica"){
            speciesIndex = 2;
        }

        speciesInfo[speciesIndex][0].addValue(sepalLength); // add values to info objects of speciesInfo vector, for each species and each statistic
        speciesInfo[speciesIndex][1].addValue(sepalWidth);
        speciesInfo[speciesIndex][2].addValue(petalLength);
        speciesInfo[speciesIndex][3].addValue(petalWidth);

    }

    file.close();
    printTable(speciesInfo, speciesNames);

    return 0;
 
}