#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

// calculate the mean
double calculateMean(const vector<double>& data) {
    double sum = 0.0;
    for (double val : data) {
        sum += val;
    }
    return sum / data.size();
}

// linear regression function
vector<double> linearRegression(const vector<double>& x, const vector<double>& y) {

    double xMean = calculateMean(x);
    double yMean = calculateMean(y);

    // calculate beta (slope) and alpha (intercept)
    double numerator = 0.0, denominator = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - xMean) * (y[i] - yMean);
        denominator += (x[i] - xMean) * (x[i] - xMean);
    }
    double beta = numerator / denominator;
    double alpha = yMean - beta * xMean;

    // calculate predicted y values and R-squared
    vector<double> predictedY;
    double SSres = 0.0, SStot = 0.0;
    for (size_t i = 0; i < y.size(); ++i) {
        double predY = alpha + beta * x[i];
        predictedY.push_back(predY);
        SSres += (y[i] - predY) * (y[i] - predY);
        SStot += (y[i] - yMean) * (y[i] - yMean);
    }
    double rSquared = 1 - (SSres / SStot);

    // print it out
    cout << fixed << setprecision(2)
         << "Beta (Slope) = " << beta << endl
         << "Alpha (Intercept) = " << alpha << endl
         << "R-squared = " << rSquared << endl;

    return predictedY;
}

// function to read csv data
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
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

int main() {
    string filename = "bug-attraction.csv";
    vector<vector<string>> data = readCSV(filename);

    // vectors to store data for each location
    vector<double> moon_phase_bg, total_bugs_bg;
    vector<double> moon_phase_combined, total_bugs_combined;

    for (size_t i = 1; i < data.size(); ++i) {
            string location = data[i][2];          // column for location
            double moon_phase = stod(data[i][5]); // column for standardized moon
            int total_bugs = stoi(data[i][32]);   // column for total bugs

            if (location == "BG") {
                moon_phase_bg.push_back(moon_phase);
                total_bugs_bg.push_back(total_bugs);
            } else if (location == "LK" || location == "Stunt") {
                moon_phase_combined.push_back(moon_phase);
                total_bugs_combined.push_back(total_bugs);
            }
    }

    cout << "BG Site Linear Regression Results:" << endl;
    auto predictedY_bg = linearRegression(moon_phase_bg, total_bugs_bg);

    cout << "LK and Stunt Sites Linear Regression Results:" << endl;
    auto predictedY_combined = linearRegression(moon_phase_combined, total_bugs_combined);

    // scatter plot for BG
    auto fig1 = figure(true);
    auto ax1 = fig1->current_axes();
    scatter(ax1, moon_phase_bg, total_bugs_bg);
    hold(on);
    plot(ax1, moon_phase_bg, predictedY_bg);
    ax1->title("BG Site");
    ax1->xlabel("Standardized Moon");
    ax1->ylabel("Total Bugs");
    text(ax1, 0.1, max(total_bugs_bg) * 0.9, "y = 6.02 + 5.30x, R^2 = 0.01"); // hard-coded from couts above
    show();

    // scatter plot for LK/stunt
    auto fig2 = figure(true);
    auto ax2 = fig2->current_axes();
    scatter(ax2, moon_phase_combined, total_bugs_combined);
    hold(on);
    plot(ax2, moon_phase_combined, predictedY_combined);
    ax2->title("LK and Stunt Sites");
    ax2->xlabel("Standardized Moon");
    ax2->ylabel("Total Bugs");
    text(ax2, 0.1, max(total_bugs_combined) * 0.9, "y = 1.65 + 6.34x, R^2 = 0.15"); // hard-coded from couts above
    show();

    return 0;
}
