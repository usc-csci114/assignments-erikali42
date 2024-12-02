#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

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
    string filename = "bug-attraction.csv";  // Path to your CSV file
    vector<vector<string>> data = readCSV(filename);

    // count the occurrences of each insect order across all light types
    unordered_map<string, int> orderCount;

    // extract insect orders from columns 18 - 31 
    for (size_t i = 1; i < data.size(); ++i) {
        for (int j = 17; j <= 30; ++j) {
            string insect_order = data[i][j];
            if (!insect_order.empty()) {
                orderCount[insect_order]++;
            }
        }
    }

    // sort insect orders by frequency and select the top 4
    vector<pair<string, int>> sortedOrders(orderCount.begin(), orderCount.end());
    sort(sortedOrders.begin(), sortedOrders.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    // get the 4 most common insect orders
    vector<string> topOrders;
    for (int i = 0; i < 4 && i < sortedOrders.size(); ++i) {
        topOrders.push_back(sortedOrders[i].first);
    }

    // manually prints out top orders, giving their indexes = 0, 1, 2, 4 = diptera, hymenoptera, hemiptera, coleoptera
    for(int i = 0; i < topOrders.size(); ++i){
        cout << topOrders[i] << endl;
    }

    // prepare the data for the grouped bar chart
    unordered_map<string, unordered_map<string, int>> lightTypeOrderCount;

    // loop through the dataset again to count the occurrences of each of the 4 most common insect orders by light type
    for (size_t i = 1; i < data.size(); ++i) {
        string light_type = data[i][0];  // light type column
        
        // loop over columns 18 - 31 
        for (int j = 17; j <= 30; ++j) {
            string insect_order = data[i][j];

            // only count for the top 4 orders
            if (find(topOrders.begin(), topOrders.end(), insect_order) != topOrders.end() && !insect_order.empty()) {
                lightTypeOrderCount[light_type][insect_order]++;
            }
        }
    }

    // prepare the data for plotting
    vector<string> lightTypes;
    vector<vector<int>> counts(topOrders.size(), vector<int>());  // to store counts for each of the 4 orders

    vector<string> predefinedOrder = {"No Light", "A", "B", "C", "CFL", "LED"};
    for (const string& lightType : predefinedOrder) {
        lightTypes.push_back(lightType);
        for (int i = 0; i < topOrders.size(); ++i) {
            string order = topOrders[i];
            counts[i].push_back(lightTypeOrderCount[lightType][order]); 
        }
    }

    // create bar graph
    auto fig = figure(true);
    auto ax = fig->current_axes();

    bar(ax, counts);
    gca()->x_axis().ticklabels(predefinedOrder);

    // set labels for the axes and title
    ax->title("Insect Orders by Light Type");
    ax->xlabel("Light Type");
    ax->ylabel("Count");

    // create legend with the top 4 insect orders - taken from cout statement that gave index of top insect orders
    text(5, 380, "Blue = Diptera");
    text(5, 368, "Orange = Hymenoptera");
    text(5, 356, "Yellow = Hemiptera");
    text(5, 344, "Purple = Coleoptera");

    show();

    return 0;
}
