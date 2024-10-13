#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Element {
	Element() :
	symbol(),name(),number(0),atomic_mass(0.0),type() {};
	
	string symbol; //e.g "H", "He"
	string name; //e.g "Hydrogen", "Helium"
	int number; //e.g 1, 2
	double atomic_mass; //e.g. 1.008, 4.0026
	string phase; //phase at room temp e.g gas, liquid, solid
	string type; //e.g. "metalliod, "transitional metals"
};

// prototypes
size_t index_find(const vector<pair<string, int> >& index, const string& key);
void load_elements(vector<Element>& elements, vector< pair<string, int> >& name_index, vector< pair<string, int> >& symbol_index, const string& filename);

struct compclass { // comparing first element of pairs
    bool operator() (pair<string, int> a, pair<string, int> b) { return (a.first < b.first); }
} comp;

int main(int argc, char* argv[]) {

    vector<Element> elements(119); // initialize vector of elements
    vector<pair<string, int> > name_index; // vector for element name and atomic weight
    vector<pair<string, int> > symbol_index; // vector for element symbol and weight
    
    // loading elements from file
    string filename = argv[1];
    load_elements(elements, name_index, symbol_index, filename);
    
    // sorting indexes of vectors
    sort(name_index.begin(), name_index.end(), comp);
    sort(symbol_index.begin(), symbol_index.end(), comp);

    // search loop
    string input;
    while (true) {
        cout << "Enter element number (1-118), symbol, or name (or type 'quit' to exit): ";
        cin >> input;

        if (input == "quit") {
            break;
        }

        // check if input is a number (search by atomic number)
        try {
            int number = stoi(input);
            if (number >= 1 && number <= 118) { // if element is a number
                Element e = elements[number];
                cout << "Element #" << e.number << ": " << e.name << " (" << e.symbol << ")" << endl;
                cout << "Atomic Mass: " << e.atomic_mass << ", Phase: " << e.phase << ", Type: " << e.type << endl;
            } else {
                cout << "Invalid atomic number. Try again." << endl;
            }
        } catch (...) {
            // if not a number, search by symbol or name
            if (input.size() <= 2) {
                // search by smybol
                size_t index = index_find(symbol_index, input);
                if (index != string::npos) {
                    Element e = elements[index];
                    cout << "Element #" << e.number << ": " << e.name << " (" << e.symbol << ")" << endl;
                    cout << "Atomic Mass: " << e.atomic_mass << ", Phase: " << e.phase << ", Type: " << e.type << endl;
                } else {
                    cout << "Element not found." << endl;
                }
            } else {
                // search by name
                size_t index = index_find(name_index, input);
                if (index != string::npos) {
                    Element e = elements[index];
                    cout << "Element #" << e.number << ": " << e.name << " (" << e.symbol << ")" << endl;
                    cout << "Atomic Mass: " << e.atomic_mass << ", Phase: " << e.phase << ", Type: " << e.type << endl;
                } else {
                    cout << "Element not found." << endl;
                }
            }
        }
    }
    return 0;
}

// function to load elements from csv file
void load_elements(vector<Element>& elements, vector<pair<string, int> >& name_index, vector<pair<string, int> >& symbol_index, const string& filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // skip the first line which is a header

    while (getline(file, line)) {
        stringstream ss(line);
        Element element;

        // read element in number, name, symbol, mass, phase, type order
        ss >> element.number;
        ss.ignore(1); // skipping commas
        getline(ss, element.name, ',');
        getline(ss, element.symbol, ',');
        ss >> element.atomic_mass;
        ss.ignore(1); // skipping comma
        getline(ss, element.phase, ',');
        getline(ss, element.type, ',');

        // store element in vector
        elements[element.number] = element; // atomic num is index
        name_index.push_back(make_pair(element.name, element.number));
        symbol_index.push_back(make_pair(element.symbol, element.number));
    }

    file.close();
}


// binary search function
size_t index_find(const vector<pair<string, int> >& index, const string& key) {
    int left = 0;
	int right = index.size() - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (index[mid].first == key) {
            return index[mid].second;
        } else if (index[mid].first < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return string::npos; // element not found
}
