// #include <iostream>
// #include <string>
// #include <fstream>
// #include <vector>
// #include <set>
// #include <map>
// #include <cmath>
// #include <limits>
// #include <sstream> // Include this header for stringstream

// using namespace std;

// ///////////////////////////////////////////////////////////////////////////
// // Function Declarations
// void PrintData(const vector<vector<string>>& table);
// void CountUniqueAttributes(const vector<vector<string>>& table);
// double getStatisticalError(double f, int N);
// double CalculateEntropy(const vector<vector<string>>& data, int targetIndex);
// vector<vector<vector<string>>> SplitDataSet(const vector<vector<string>>& data, int featureIndex, const string& value);
// pair<string, double> FindBestSplit(const vector<vector<string>>& data, int featureIndex);

// /////////////////////////////////////////////////////////////////////////////
// class Node {
//     int Index; // Index of the feature being used for the split
//     string feature_name; // Name of the feature
//     Node* left; // Pointer to left child
//     Node* right; // Pointer to right child
//     string result; // Result if the node is a leaf

// public:
//     // Constructor for creating a node
//     Node(int index, Node* left = nullptr, Node* right = nullptr)
//         : Index(index), left(left), right(right) {} // Member initializer list
// };

// class DecisionTree {
//     Node* root = nullptr; // Root of the decision tree
// };

// class Input {
//     vector<vector<string>> table; // 2D vector to hold the dataset
//     int T_row = 0; // Counter for the number of rows in the dataset

// public:
//     // Constructor to read data from a specified file
//     Input(string f_name) {
//         ifstream reader(f_name); // Open the file for reading

//         // Check if the file is successfully opened
//         if (!reader.is_open()) {
//             cout << "File does not exist;" << endl;
//             return; // Exit if the file cannot be opened
//         }

//         string line; // Variable to hold each line of the CSV

//         // Read the file line by line
//         while (getline(reader, line)) {
//             vector<string> row; // Create a new row for each line
//             stringstream ss(line); // Use stringstream to parse the line
//             string value; // Variable to hold each value in the row

//             // Split the line by comma
//             while (getline(ss, value, ',')) {
//                 row.push_back(value); // Add the parsed value to the row
//             }

//             // Only add the row if it has at least one value
//             if (!row.empty()) {
//                 table.push_back(row); // Add the row to the table
//                 T_row++; // Increment the row counter
//             }
//         }

//         // Close the reader after finishing
//         reader.close(); // Close the file
//     }

//     // Function to get the total number of rows in the table
//     int GetRow() const {
//         return T_row; // Return the count of rows
//     }

//     // Function to get the table data
//     vector<vector<string>> GetTable() const {
//         return table; // Return the 2D vector holding the data
//     }
// };

// // Calculate entropy of a dataset for a specific target index
// double CalculateEntropy(const vector<vector<string>>& data, int targetIndex) {
//     map<string, int> labelCounts; // Map to hold counts of each label
//     for (const auto& row : data) {
//         if (targetIndex < row.size()) { // Ensure targetIndex is within bounds
//             labelCounts[row[targetIndex]]++; // Count the occurrences of each label
//         }
//     }

//     double entropy = 0.0; // Variable to hold the calculated entropy
//     int total = data.size(); // Total number of rows in the dataset
//     for (const auto& count : labelCounts) {
//         double probability = static_cast<double>(count.second) / total; // Calculate probability
//         entropy -= probability * log2(probability); // Calculate entropy using the formula
//     }

//     return entropy; // Return the calculated entropy
// }

// // Split the dataset based on a feature and a given value
// vector<vector<vector<string>>> SplitDataSet(const vector<vector<string>>& data, int featureIndex, const string& value) {
//     vector<vector<string>> left, right; // Vectors to hold split datasets
//     for (const auto& row : data) {
//         if (featureIndex < row.size()) { // Check if featureIndex is within bounds
//             if (row[featureIndex] == value) {
//                 left.push_back(row); // Add to left if it matches the value
//             } else {
//                 right.push_back(row); // Add to right otherwise
//             }
//         }
//     }
//     return {left, right}; // Return both left and right subsets
// }

// // Find the best split point for a given feature by calculating entropy
// pair<string, double> FindBestSplit(const vector<vector<string>>& data, int featureIndex) {
//     set<string> uniqueValues; // Set to hold unique values for the feature
//     for (const auto& row : data) {
//         if (featureIndex < row.size()) { // Check if featureIndex is within bounds
//             uniqueValues.insert(row[featureIndex]); // Collect unique values for the feature
//         }
//     }

//     string bestValue; // Variable to hold the best value for the split
//     double bestEntropy = numeric_limits<double>::max(); // Initialize best entropy to maximum value

//     for (const auto& value : uniqueValues) {
//         auto subsets = SplitDataSet(data, featureIndex, value); // Split the data

//         double totalSize = data.size(); // Total size of the dataset
//         double leftSize = subsets[0].size(); // Size of the left subset
//         double rightSize = subsets[1].size(); // Size of the right subset

//         // Calculate weighted entropy for this split
//         double weightedEntropy = (leftSize / totalSize) * CalculateEntropy(subsets[0], data[0].size() - 1) +
//                                  (rightSize / totalSize) * CalculateEntropy(subsets[1], data[0].size() - 1);

//         // Check if this split is better
//         if (weightedEntropy < bestEntropy) {
//             bestEntropy = weightedEntropy; // Update best entropy
//             bestValue = value; // Update best value
//         }
//     }

//     return {bestValue, bestEntropy}; // Return the value and entropy of the best split
// }

// void PrintData(const vector<vector<string>>& table) {
//     for (const auto& row : table) {
//         for (const auto& elem : row) {
//             cout << elem << ','; // Print each element followed by a comma
//         }
//         cout << endl; // Print a new line after each row
//     }
// }

// void CountUniqueAttributes(const vector<vector<string>>& table) {
//     if (table.empty()) return; // Return if the table is empty

//     vector<set<string>> uniqueAttributes; // Vector of sets to hold unique attributes for each column
//     for (const auto& row : table) {
//         for (size_t col = 0; col < row.size(); ++col) {
//             if (col >= uniqueAttributes.size()) {
//                 uniqueAttributes.resize(col + 1); // Resize to accommodate new column
//             }
//             uniqueAttributes[col].insert(row[col]); // Add unique attributes for each column
//         }
//     }

//     // Print unique counts for each column
//     for (size_t col = 0; col < uniqueAttributes.size(); ++col) {
//         cout << "Column " << col + 1 << " has " << uniqueAttributes[col].size() << " unique attributes." << endl;
//     }
// }

// double getStatisticalError(double f, int N) {
//     double z = 1.96; // 95% Confidence Interval
//     if (N == 0) {
//         cout << "Error Located in function for statistical error" << endl;
//         cout << "N is zero, ensure N is a valid number" << endl;
//         exit(0); // Exit if N is zero
//     }
//     return (f + z * z / (2 * N) + z * sqrt(f / N - f * f / N + z * z / (4 * N * N))) / (1 + z * z / N);
// }

// int main(int argc, const char* argv[]) {
//     cout << "argc value: " << argc << endl; // Print argument count
//     cout << "argv[0]: " << argv[0] << endl; // Print program name

//     string f_name; // Variable to hold the file name
//     cout << "Enter Your File Name" << endl; // Prompt user for file name
//     cin >> f_name; // Read file name from user input

//     Input train_file(f_name); // Create Input object to read the file
//     cout << "get Row: " << train_file.GetRow() << endl; // Print number of rows read

//     PrintData(train_file.GetTable()); // Print the contents of the dataset
//     CountUniqueAttributes(train_file.GetTable()); // Count and print unique attributes

//     // Calculate the index of the last column as target index
//     int targetIndex = train_file.GetTable()[0].size() - 1; // Set target index to the last column
//     double entropy = CalculateEntropy(train_file.GetTable(), targetIndex); // Calculate entropy for the target column
//     cout << "BaseEntropy: " << entropy << endl; // Print the calculated entropy

//     // Example of finding the best split for a feature, say column 0
//     int featureIndex = 0; // Set feature index to the first column
//     auto bestSplit = FindBestSplit(train_file.GetTable(), featureIndex); // Find the best split
//     cout << "Best split value for feature " << featureIndex << " is: " << bestSplit.first 
//          << " with entropy: " << bestSplit.second << endl; // Print best split details

//     return 0; // Exit the program
// }
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <limits>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Function Declarations
void PrintData(const vector<vector<string>>& table);
void CountUniqueAttributes(const vector<vector<string>>& table);
double getStatisticalError(double f, int N);
double CalculateEntropy(const vector<vector<string>>& data, int targetIndex);
map<string, vector<vector<string>>> SplitDataSet(const vector<vector<string>>& data, int featureIndex);
pair<string, double> FindBestSplit(const vector<vector<string>>& data, int featureIndex);

/////////////////////////////////////////////////////////////////////////////
class Node {
    int Index;  // Index of the feature being used for the split
    string feature_name; // Name of the feature
    string result;  // Result if the node is a leaf
    map<string, Node*> children; // Map to hold children based on unique attribute values

public:
    // Constructor for creating a node
    Node(int index, string feat_name) 
        : Index(index), feature_name(feat_name) {}

    void addChild(const string& value, Node* child) {
        children[value] = child;
    }

    bool isLeaf() const {
        return children.empty();
    }

    Node* getChild(const string& value) const {
        auto it = children.find(value);
        return (it != children.end()) ? it->second : nullptr;
    }

    const map<string, Node*>& getChildren() const {
        return children;
    }
};

class DecisionTree {
    Node* root = nullptr;

public:
    DecisionTree(Node* rootNode) : root(rootNode) {}

    // Tree traversal or other methods can be added here
};

class Input {
    vector<vector<string>> table;
    int T_row = 0;

public:
    Input(string f_name) {
        ifstream reader(f_name);
        if (!reader.is_open()) {
            cout << "File does not exist;" << endl;
            return;
        }

        string line;
        while (getline(reader, line)) {
            vector<string> row;
            stringstream ss(line);
            string value;
            while (getline(ss, value, ',')) {
                row.push_back(value);
            }
            if (!row.empty()) {
                table.push_back(row);
                T_row++;
            }
        }
        reader.close();
    }

    int GetRow() const {
        return T_row;
    }

    vector<vector<string>> GetTable() const {
        return table;
    }
};

// Calculate entropy for the target index
double CalculateEntropy(const vector<vector<string>>& data, int targetIndex) {
    map<string, int> labelCounts;
    for (const auto& row : data) {
        if (targetIndex < row.size()) {
            labelCounts[row[targetIndex]]++;
        }
    }

    double entropy = 0.0;
    int total = data.size();
    for (const auto& count : labelCounts) {
        double probability = static_cast<double>(count.second) / total;
        entropy -= probability * log2(probability);
    }

    return entropy;
}

// Split the dataset based on a feature and all its unique values (B-tree branching)
map<string, vector<vector<string>>> SplitDataSet(const vector<vector<string>>& data, int featureIndex) {
    map<string, vector<vector<string>>> subsets;
    for (const auto& row : data) {
        if (featureIndex < row.size()) {
            subsets[row[featureIndex]].push_back(row);
        }
    }
    return subsets;
}

// Find the best split based on unique values for a feature
pair<string, double> FindBestSplit(const vector<vector<string>>& data, int featureIndex) {
    set<string> uniqueValues;
    for (const auto& row : data) {
        if (featureIndex < row.size()) {
            uniqueValues.insert(row[featureIndex]);
        }
    }

    double totalSize = data.size();
    double bestEntropy = numeric_limits<double>::max();
    string bestValue;

    for (const auto& value : uniqueValues) {
        auto subsets = SplitDataSet(data, featureIndex);
        double weightedEntropy = 0.0;
        
        for (const auto& subset : subsets) {
            double subsetSize = subset.second.size();
            weightedEntropy += (subsetSize / totalSize) * CalculateEntropy(subset.second, data[0].size() - 1);
        }

        if (weightedEntropy < bestEntropy) {
            bestEntropy = weightedEntropy;
            bestValue = value;
        }
    }

    return {bestValue, bestEntropy};
}

void PrintData(const vector<vector<string>>& table) {
    for (const auto& row : table) {
        for (const auto& elem : row) {
            cout << elem << ',';
        }
        cout << endl;
    }
}

void CountUniqueAttributes(const vector<vector<string>>& table) {
    if (table.empty()) return;

    vector<set<string>> uniqueAttributes;
    for (const auto& row : table) {
        for (size_t col = 0; col < row.size(); ++col) {
            if (col >= uniqueAttributes.size()) {
                uniqueAttributes.resize(col + 1);
            }
            uniqueAttributes[col].insert(row[col]);
        }
    }

    for (size_t col = 0; col < uniqueAttributes.size(); ++col) {
        cout << "Column " << col + 1 << " has " << uniqueAttributes[col].size() << " unique attributes." << endl;
    }
}

double getStatisticalError(double f, int N) {
    double z = 1.96;
    if (N == 0) {
        cout << "Error in function for statistical error: N is zero, ensure N is a valid number" << endl;
        exit(0);
    }
    return (f + z * z / (2 * N) + z * sqrt(f / N - f * f / N + z * z / (4 * N * N))) / (1 + z * z / N);
}

int main(int argc, const char* argv[]) {
    cout << "argc value: " << argc << endl;
    cout << "argv[0]: " << argv[0] << endl;

    string f_name;
    cout << "Enter Your File Name" << endl;
    cin >> f_name;

    Input train_file(f_name);
    cout << "get Row: " << train_file.GetRow() << endl;

    PrintData(train_file.GetTable());
    CountUniqueAttributes(train_file.GetTable());

    int targetIndex = train_file.GetTable()[0].size() - 1;
    double entropy = CalculateEntropy(train_file.GetTable(), targetIndex);
    cout << "BaseEntropy: " << entropy << endl;

    int featureIndex = 0;
    auto bestSplit = FindBestSplit(train_file.GetTable(), featureIndex);
    cout << "Best split value for feature " << featureIndex << " is: " << bestSplit.first 
         << " with entropy: " << bestSplit.second << endl;

    return 0;
}
