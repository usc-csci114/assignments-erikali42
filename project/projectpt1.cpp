#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

// Function to read CSV data
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

// Function to count occurrences for a column (e.g., baldness status)
map<string, int> countOccurrences(const vector<vector<string>>& data, size_t colIndex) {
    map<string, int> counts;
    for (size_t i = 1; i < data.size(); ++i) { // Start at 1 to skip header
        if (colIndex < data[i].size()) {
            counts[data[i][colIndex]]++;
        }
    }
    return counts;
}

int main() {
    string filename = "hair_health.csv"; 
    auto data = readCSV(filename);

    // Plot 1: Bar Plot for Baldness Status
    size_t baldnessColumnIndex = 12; 
    auto baldnessCounts = countOccurrences(data, baldnessColumnIndex);

    vector<double> x; // Numeric indices for bars
    vector<double> values;
    vector<string> labels;

    size_t index = 0;
    for (const auto& pair : baldnessCounts) {
        x.push_back(index++); // Assign a numeric index
        values.push_back(pair.second);
        labels.push_back(pair.first);
    }

    auto bars = bar(x, values);
    xlabel("Baldness Status");
    ylabel("Count");
    title("Baldness Status Distribution");

    // Set the string labels on the x-axis
    gca()->x_axis().ticklabels({"No Hair Loss", "Hair Loss"});
    show();

    // clearing plot
    cla();
    auto ax = gca();  
    ax->x_axis().ticklabels({});  

    // Plot 2: Histogram for Age and Hair Loss
    vector<double> ageWithHairLoss;
    size_t ageIndex = 7, hairLossIndex = 12; 

    for (size_t i = 1; i < data.size(); ++i) {
        if (ageIndex < data[i].size() && hairLossIndex < data[i].size()) {
            double age = stod(data[i][ageIndex]);
            int hairLoss = stoi(data[i][hairLossIndex]);
            if (hairLoss == 1) {
                ageWithHairLoss.push_back(age);
            } 
        }
    }

    auto h1 = hist(ageWithHairLoss, 10);  // Create the histogram for age with hair loss
    gca()->xlim({18, 50});  // Set x-axis range to match the age data range
    gca()->xticks({20, 25, 30, 35, 40, 45, 50}); 
    xlabel("Age");
    ylabel("Frequency");
    title("Age Distribution for Individuals with Hair Loss");
    show();

    // clear plot
    auto ax1 = gca();  
    ax1->x_axis().ticklabels({});  
    cla();


    // Plot 3: Stacked Bar Plot: Hair Loss by Stress Level
    size_t stressIndex = 6; 
    map<string, vector<int>> hairLossByStress;

    // Populate hairLossByStress map
    for (size_t i = 1; i < data.size(); ++i) {
        if (stressIndex < data[i].size() && data[i].size() > 12) {
            string stressLevel = data[i][stressIndex];
            int hairLoss = stoi(data[i][12]); // Column index for "Hair Loss"
            hairLossByStress[stressLevel].push_back(hairLoss);
        }
    }

    // Prepare data for stacked bar plot
    vector<string> stressLevels = {"Low", "Moderate", "High"};
    vector<double> noHairLoss, hairLoss;

    for (const auto& level : stressLevels) {
        auto& hairLossValues = hairLossByStress[level];
        int countNo = count(hairLossValues.begin(), hairLossValues.end(), 0);
        int countYes = count(hairLossValues.begin(), hairLossValues.end(), 1);
        noHairLoss.push_back(countNo);
        hairLoss.push_back(countYes);
    }

    // Create a 2D vector where each row corresponds to a data series
    vector<vector<double>> Y = {noHairLoss, hairLoss};

    // Generate stacked bar plot
    auto h = barstacked(Y);
    gca()->x_axis().ticklabels(stressLevels); // Set x-axis labels to stress levels
    xlabel("Stress Level");
    ylabel("Count");
    title("Hair Loss by Stress Level");
    matplot::legend({"No Hair Loss", "Hair Loss"});
    show();

    // Clearing plot
    auto ax2 = gca();  
    ax2->x_axis().ticklabels({});  
    cla();

    // Plot 4: Stacked Bar Plot - Genetics vs Hair Loss
    size_t geneticsIndex = 1;  

    // Map to store Genetics counts grouped by Hair Loss
    map<string, vector<int>> geneticsByHairLoss;

    // Populate the geneticsByHairLoss map
    for (size_t i = 1; i < data.size(); ++i) {
        if (hairLossIndex < data[i].size() && geneticsIndex < data[i].size()) {
            string hairLoss = data[i][hairLossIndex];
            string genetics = data[i][geneticsIndex];

            // Initialize if not already in the map
            if (geneticsByHairLoss.find(hairLoss) == geneticsByHairLoss.end()) {
                geneticsByHairLoss[hairLoss] = {0, 0}; // {No Genetics, Yes Genetics}
            }

            // Increment counts based on Genetics (Yes/No)
            if (genetics == "Yes") {
                geneticsByHairLoss[hairLoss][1]++;
            } else if (genetics == "No") {
                geneticsByHairLoss[hairLoss][0]++;
            }
        }
    }

    // Prepare data for the stacked bar plot
    // Automatically determine Hair Loss categories from the map keys
    vector<string> hairLossCategories;
    for (const auto& [hairLoss, counts] : geneticsByHairLoss) {
        hairLossCategories.push_back(hairLoss);
    }

    // Prepare data for the stacked bar plot
    vector<double> noGenetics, yesGenetics;

    for (const auto& category : hairLossCategories) {
        // Extract counts for each category
        noGenetics.push_back(geneticsByHairLoss[category][0]);
        yesGenetics.push_back(geneticsByHairLoss[category][1]);
    }

    // Create a 2D vector for stacked bar plot
    vector<vector<double>> yVar = {noGenetics, yesGenetics};

    // Generate the stacked bar plot
    auto h2 = barstacked(yVar);
    vector<string> x_axis = {"No Hair Loss", "Hair Loss"};
    gca()->x_axis().ticklabels(x_axis); // Dynamic tick labels
    xlabel("Hair Loss");
    ylabel("Count");
    title("Genetics vs Hair Loss");
    matplot::legend({"No Family History", "Family History"});
    show();

    // Clearing plot
    auto ax3 = gca();  
    ax3->x_axis().ticklabels({}); 
    cla();

    // Plot 5: Stacked Bar Plot - Hair Loss vs Smoking
    size_t smokingIndex = 10;  

    // Map to store Smoking counts grouped by Hair Loss
    map<string, vector<int>> smokingByHairLoss;

    // Populate the smokingByHairLoss map
    for (size_t i = 1; i < data.size(); ++i) {
        if (hairLossIndex < data[i].size() && smokingIndex < data[i].size()) {
            string hairLoss = data[i][hairLossIndex];
            string smoking = data[i][smokingIndex];

            // Initialize if not already in the map
            if (smokingByHairLoss.find(hairLoss) == smokingByHairLoss.end()) {
                smokingByHairLoss[hairLoss] = {0, 0}; 
            }

            // Increment counts based on Smoking status (Yes/No)
            if (smoking == "Yes") {
                smokingByHairLoss[hairLoss][1]++;
            } else if (smoking == "No") {
                smokingByHairLoss[hairLoss][0]++;
            }
        }
    }

    // Prepare data for the stacked bar plot
    vector<double> noSmoke, yesSmoke;

    for (const auto& category : hairLossCategories) {
        // Extract counts for each category
        noSmoke.push_back(smokingByHairLoss[category][0]);
        yesSmoke.push_back(smokingByHairLoss[category][1]);
    }

    // Create a 2D vector for stacked bar plot
    vector<vector<double>> yVariable = {noSmoke, yesSmoke};

    // Generate the stacked bar plot
    auto h3 = barstacked(yVariable);
    gca()->x_axis().ticklabels(x_axis);
    xlabel("Hair Loss");
    ylabel("Count");
    title("Smoking vs Hair Loss");
    matplot::legend({"Non-smoker", "Smoker"});
    show();


    return 0;
}

