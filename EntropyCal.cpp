#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include<map>
#include<cmath>

using namespace std;

double CalculateEntropy(const vector<vector<string>>& data, int targetIndex) {
    map<string, int> labelCounts;
    for (const auto& row : data) {
        labelCounts[row[targetIndex]]++;
    }

    double entropy = 0.0;
    int total = data.size();
    for (const auto& count : labelCounts) {
        double probability = static_cast<double>(count.second) / total;
        entropy -= probability * log2(probability);
    }

    return entropy;
}