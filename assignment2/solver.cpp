#include <iostream>
#include <vector>
#include "system.h"

using namespace std;

int main() {
    size_t n, m;
    
    // ask for user input
    cout << "Enter the number of equations (n): ";
    cin >> n;
    cout << "Enter the number of unknowns (m): ";
    cin >> m;
    
    // initialize a 2D vector (n rows, m+1 columns) for the augmented matrix
    vector<vector<double> > matrix(n, vector<double>(m + 1));
    
    // ask the user to enter the augmented matrix row by row
    cout << "Enter the augmented matrix (each row should have " << m + 1 << " values):\n";
    for (size_t i = 0; i < n; ++i) {
        cout << "Row " << i + 1 << ": ";
        for (size_t j = 0; j < m + 1; ++j) {
            cin >> matrix[i][j];
        }
    }
    
    // create a System object using the input data
    System system(n, m, matrix);
    
    // call the solve function to solve the system of equations
    system.solve();
    
    // determine the number of solutions and output the result
    int numSolutions = system.getNumSolutions();
    
    if (numSolutions == 0) {
        cout << "The system has no solutions.\n";
    } else if (numSolutions == 1) {
        cout << "The system has one solution:\n";
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < solution.size(); ++i) {
            cout << "x" << i + 1 << " = " << solution[i] << "\n";
        }
    } else if (numSolutions == 2) {
        cout << "The system has infinite solutions. One possible solution is:\n";
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < solution.size(); ++i) {
            cout << "x" << i + 1 << " = " << solution[i] << "\n";
        }
    }
    
    return 0;
}
