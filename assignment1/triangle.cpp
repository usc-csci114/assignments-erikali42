#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std; 

// calculating triangle area given coordinates
double calculateTriangleArea(double x1, double y1, double x2, double y2, double x3, double y3){
    return abs(0.5*((x1*(y2 - y3)) + (x2*(y3 - y1)) + (x3*(y1 - y2))));
}

// receive input and check for input errors
double getTriangleCoordinates(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3){
    bool goodInput = false;
    while(!goodInput){
        cout << "Enter in the triangle coordinates or 'quit' to exit" << endl;
        string input;
        getline(cin, input);
        if(input == "quit"){
            return false;
        }

        stringstream inputStream(input);
        double tempX1, tempY1, tempX2, tempY2, tempX3, tempY3;
        if(inputStream >> tempX1 >> tempY1 >> tempX2 >> tempY2 >> tempX3 >> tempY3){
            x1 = tempX1;
            y1 = tempY1;
            x2 = tempX2;
            y2 = tempY2;
            x3 = tempX3;
            y3 = tempY3;
            goodInput = true;
            return true; 
        }
        else{
            cout << "Invalid input. Enter in the triangle coordinates or 'quit' to exit" << endl;
        }
    }
    return false; 
}

int main(){
    double x1, y1, x2, y2, x3, y3;
    bool continueLoop = true;
    while(continueLoop){
        if(getTriangleCoordinates(x1, y1, x2, y2, x3, y3)){
            double area = calculateTriangleArea(x1, y1, x2, y2, x3, y3);
            cout << "The area of the triangle is " << area << endl;
        }
        else{
            cout << "Ending program." << endl;
            continueLoop = false; 
        }
    }
    return 0;
}


