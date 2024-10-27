#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include<bits/stdc++.h>


using namespace std;

// Function to count Yes and No for each weather condition
void Information_Gain(const vector<vector<string>>& data) {
    // Map to store counts for each weather condition
    map<string, map<string, int>> conditionCounts;

    // Start from 1 to skip the header row
    for (size_t i = 1; i < data.size(); ++i) {
        string condition = data[i][0]; // Get the weather condition (Sunny, Rain, Overcast)
        string play = data[i][4]; // Get the value in the "Play" column
        
        // Increment the count for the specific condition and play
        conditionCounts[condition][play]++;
    }

    // Output the results
    vector<int> entrop;
   
   for (const auto& condition : conditionCounts) {
        cout << condition.first << " - ";
         double overallEntropy = 0.0;
        int total = 0;
        for (const auto& playCount : condition.second) {
            total = total + playCount.second ;
            
            cout << playCount.first << ": " << playCount.second << " ";
         (overallEntropy -= (playCount.second / ( total ) ) * log2(playCount.second / (total ) ))  ;
        }
        cout << "overallEntropy "<< overallEntropy << endl ;
        cout << "total :"<<total << endl ;
        cout << endl; // Newline after each condition
      
    }
    for(auto &itr :entrop)
    {
        cout << entrop[itr]<< "^^^^" << endl;
    }
    
    
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
    
    // Count Yes and No in the overall dataset
    map<string, int> overallCounts;
    for (size_t i = 1; i < data.size(); ++i) {
        string play = data[i][4]; // Get the value in the "Play" column
        overallCounts[play]++; // Increment the count for "Yes" or "No"
    }
    double totalYes = overallCounts["Yes"];
    double totalNo = overallCounts["No"];
    double total = totalYes + totalNo;

    double overallEntropy = 0.0;
    if (totalYes > 0) overallEntropy -= (totalYes / total) * log2(totalYes / total);
    if (totalNo > 0) overallEntropy -= (totalNo / total) * log2(totalNo / total);
    cout << "Overall Entropy: " << overallEntropy << endl;
   
    // Output the overall results
    cout << "Overall - Yes: " << overallCounts["Yes"] << ", No: " << overallCounts["No"] << endl;

    // Call the function to count Yes and No by weather condition
    Information_Gain(data);

    return 0;
}
