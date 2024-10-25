#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Function Declarations

// Prints the entire dataset stored in a 2D vector (table)
void PrintData(const vector<vector<string>> &table);

// Counts unique attributes (unique values in each column) in the dataset
void CountUniqueAttributes(const vector<vector<string>> &table);

// Calculates statistical error using a specific formula (confidence level and sample count)
double getStatisticalError(double f, int N);

/////////////////////////////////////////////////////////////////////////////
// Node class for building tree nodes

class Node {
    int Index;             // Index of the feature used to split at this node
    string feature_name;   // Name of the feature for splitting
    Node* left;            // Pointer to the left child node
    Node* right;           // Pointer to the right child node
    string result;         // Stores result label if it is a leaf node

public:
    // Constructor for creating a node with index and optional left and right children
    Node(int index, Node* left = nullptr, Node* right = nullptr) {
        Index = index;
        this->left = left;
        this->right = right;
    }
};

/////////////////////////////////////////////////////////////////////////////
// DecisionTree class to manage the tree

class DecisionTree {
    Node* root = nullptr;  // Root node of the decision tree
};

/////////////////////////////////////////////////////////////////////////////
// Input class to manage reading data from a file

class Input {
    vector<vector<string>> table; // Table to hold the dataset
    int T_row = 0;                // Total number of rows read from the file

public:
    // Constructor that reads the dataset from a given file
    Input(string f_name) {
        ifstream reader(f_name);  // Open file for reading
        if (reader.is_open()) {
            table.resize(1000);   // Initial maximum size of 1000 rows
            while (reader.good()) {
                table[T_row].resize(7);  // Each row has 7 columns (attributes)
                for (int i = 0; i < 7; ++i) {
                    getline(reader, table[T_row][i], (i < 7 ? ',' : '\n'));  // Read each attribute
                }
                T_row++;  // Increment row counter
            }

            // Resize table to match the exact number of rows read
            if (table.size() > T_row) {
                table.erase(table.begin() + T_row, table.end());
            }
        } else {
            cout << "File does not exist;" << endl;  // Error message if file not found
        }
    }
    
    // Getter for the number of rows in the dataset
    int GetRow() const {
        return T_row;
    }

    // Getter for the dataset
    vector<vector<string>> GetTable() const {
        return table;
    }
};

// Function to calculate the entropy of a dataset based on target labels
double CalculateEntropy(const vector<vector<string>>& data, int targetIndex) {
    map<string, int> labelCounts;  // Map to count occurrences of each target label
    for (const auto& row : data) {
        labelCounts[row[targetIndex]]++;
    }

    double entropy = 0.0;
    int total = data.size();
    for (const auto& count : labelCounts) {
        double probability = static_cast<double>(count.second) / total;  // Probability of each label
        entropy -= probability * log2(probability);  // Entropy formula
    }

    return entropy;
}

// Function to print the dataset
void PrintData(const vector<vector<string>> &table) {
    for (const auto& row : table) {
        for (const auto& elem : row) {
            cout << elem << ',';
        }
        cout << endl;
    }
}

// Function to count unique attributes in each column of the dataset
void CountUniqueAttributes(const vector<vector<string>> &table) {
    if (table.empty()) return;  // If table is empty, nothing to count

    // Create a set for each column to store unique values
    vector<set<string>> uniqueAttributes(6);  // 6 columns, excluding the label column
    cout << endl << "error " << endl ;
    for (int col = 0; col < 6; col++) {
        for (int row = 0; row < table.size(); row++) {
            uniqueAttributes[col].insert(table[row][col]);  // Insert each value in the set
        }
    }

    // Print unique attribute counts for each column
    for (size_t col = 0; col < uniqueAttributes.size(); ++col) {
        cout << "Column " << col + 1 << " has " << uniqueAttributes[col].size() << " unique attributes." << endl;
    }
}

// Function to calculate statistical error based on confidence interval and sample size
double getStatisticalError(double f, int N) {
    double z = 1.96;  // z-score for 95% confidence interval

    if (N == 0) {
        cout << "Error Located in function for statistical error" << endl;
        cout << "N is zero, ensure N is a valid number" << endl;
        exit(0);  // Exit if sample size is zero
    }

    // Statistical error formula
    return (f + z * z / (2 * N) + z * sqrt(f / N - f * f / N + z * z / (4 * N * N))) / (1 + z * z / N);
}

// Main function
int main(int argc, const char* argv[]) {
    cout << "argc value : " << argc << endl;        // Print the number of command-line arguments
    cout << "argv[0] : " << argv[0] << endl;        // Print the program name

    string f_name;
    cout << "Enter Your File Name" << endl;
    cin >> f_name;  // Take input file name from the user

    Input train_file(f_name);  // Initialize Input object to read the file
    cout << "get Row : " << train_file.GetRow() << endl;  // Print total rows in the file
    
    PrintData(train_file.GetTable());  // Print dataset content
    CountUniqueAttributes(train_file.GetTable());  // Print unique attributes count for each column
    double entropy = CalculateEntropy(train_file.GetTable(), 6);  // Calculate base entropy
    cout << "BaseEntropy : " << entropy << endl;

    return 0;
}

