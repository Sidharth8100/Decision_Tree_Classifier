#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to calculate entropy
// 


using namespace std;

// Function to calculate entropy
double calculateEntropy(const map<string, int>& counts, int total) {
    double entropy = 0.0;
    for (const auto& count : counts) {
        if (count.second > 0) {
            double probability = static_cast<double>(count.second) / total;
            entropy -= probability * log2(probability);
        }
    }
    return entropy;
}

// Function to calculate information gain for each column
vector<double> Information_Gain(const vector<vector<string>>& data, int targetIndex) {
    map<string, int> overallCounts;
    int totalData = data.size() - 1; // Exclude header

    // Count overall target values (Yes/No)
    for (size_t i = 1; i < data.size(); ++i) {
        string target = data[i][targetIndex]; // Get the value in the target column
        overallCounts[target]++;
    }

    // Calculate overall entropy
    double overallEntropy = calculateEntropy(overallCounts, totalData);
                                     // cout << "Overall Entropy: " << overallEntropy << endl;

    vector<double> informationGain(data[0].size() - 1, 0.0); // Size excludes the target column

    // Iterate over each attribute (excluding the target column)
    for (size_t col = 0; col < data[0].size() - 1; ++col) {
        map<string, map<string, int>> conditionCounts;

        // Count Yes and No for each condition in the current attribute
        for (size_t i = 1; i < data.size(); ++i) {
            string condition = data[i][col]; // Get the condition
            string target = data[i][targetIndex]; // Get the target value
            conditionCounts[condition][target]++;
        }

        // Calculate conditional entropy
        double conditionalEntropy = 0.0;

        for (const auto& condition : conditionCounts) {
            int conditionTotal = 0;
            for (const auto& count : condition.second) {
                conditionTotal += count.second;
            }

            if (conditionTotal > 0) {
                conditionalEntropy += (static_cast<double>(conditionTotal) / totalData) * 
                                      calculateEntropy(condition.second, conditionTotal);
            }
        }

      
        informationGain[col] = overallEntropy - conditionalEntropy;
                           //    cout << "Information Gain for column " << col << ": " << informationGain[col] << endl;
    }

    return informationGain;
}


