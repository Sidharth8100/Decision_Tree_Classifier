#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>

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
    // Map to store counts for overall "Play"
    map<string, int> overallCounts;
    int totalData = data.size() - 1; // Exclude header

    // Count overall target values (Yes/No)
    for (size_t i = 1; i < data.size(); ++i) {
        string target = data[i][targetIndex]; // Get the value in the target column
        overallCounts[target]++;
    }

    // Calculate overall entropy
    double overallEntropy = calculateEntropy(overallCounts, totalData);

    // Vector to store information gain for each attribute
    vector<double> informationGain(data[0].size() - 1); // Size excludes the target column

    // Iterate over each attribute (excluding the target column)
    for (size_t col = 0; col < data[0].size(); ++col) {
        if (col == targetIndex) continue; // Skip the target column

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
            int yesCount = condition.second.count("Yes") ? condition.second.at("Yes") : 0;
            int noCount = condition.second.count("No") ? condition.second.at("No") : 0;
            double conditionTotal = yesCount + noCount;

            if (conditionTotal > 0) {
                conditionalEntropy += (conditionTotal / (data.size() - 1)) * calculateEntropy(condition.second, conditionTotal);
            }
        }

        // Information Gain = Overall Entropy - Conditional Entropy
        informationGain[col < targetIndex ? col : col - 1] = overallEntropy - conditionalEntropy; // Adjust for skipped index
    }

    return informationGain;
}

int main() {
    vector<vector<string>> data = {
        {"Outlook", "Temperature", "Humidity", "Wind", "Play"},
        {"Sunny", "Hot", "High", "Weak", "No"},
        {"Sunny", "Hot", "High", "Strong", "No"},
        {"Overcast", "Hot", "High", "Weak", "Yes"},
        {"Rain", "Mild", "High", "Weak", "Yes"},
        {"Rain", "Cool", "Normal", "Weak", "Yes"},
        {"Rain", "Cool", "Normal", "Strong", "No"},
        {"Overcast", "Cool", "Normal", "Strong", "Yes"},
        {"Sunny", "Mild", "High", "Weak", "No"},
        {"Sunny", "Cool", "Normal", "Weak", "Yes"},
        {"Rain", "Mild", "Normal", "Weak", "Yes"},
        {"Sunny", "Mild", "Normal", "Strong", "Yes"},
        {"Overcast", "Mild", "High", "Strong", "Yes"},
        {"Overcast", "Hot", "Normal", "Weak", "Yes"},
        {"Rain", "Mild", "High", "Strong", "No"}
    };

    int targetIndex = 4; // Index of the target column ("Play")

    // Call the function to calculate information gain
    vector<double> gains = Information_Gain(data, targetIndex);

    // Output the information gains
    for (size_t i = 0; i < gains.size(); ++i) {
        cout << "Information Gain for column " << (i < targetIndex ? i : i + 1) << ": " << gains[i] << endl;
    }
    cout << *max_element(gains.begin(), gains.end() );

    return 0;
}
