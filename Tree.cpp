#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <iomanip>
#include "prediction.h"
#include <windows.h>
///////////////////////

#include "Information_entropy.cpp"
#include "Input_DTC.cpp"
#include "EntropyCal.cpp"

//////////////////////
using namespace std;

// Function to create a leaf node
Node* createLeafNode(const vector<vector<string>>& data, int targetIndex) {
    map<string, int> counts;
    for (size_t i = 1; i < data.size(); ++i) { // Exclude header
        counts[data[i][targetIndex]]++;
    }
    
    // Find the class with the maximum count
    string maxClass;
    int maxCount = 0;
    for (const auto& count : counts) {
        if (count.second > maxCount) {
            maxCount = count.second;
            maxClass = count.first;
        }
    }
    
    Node* leaf = new Node(-1); // -1 indicates a leaf node
    leaf->decision = maxClass;
    return leaf;
}

// Function to build the decision tree
Node* buildTree(const vector<vector<string>>& data, int targetIndex) {
    // Base case: Check stopping criteria (e.g., pure subset or no data)
    if (data.size() <= 1) { // Only header or no data
        return createLeafNode(data, targetIndex);
    }
    
    map<string, int> overallCounts;
    for (size_t i = 1; i < data.size(); ++i) {
        overallCounts[data[i][targetIndex]]++;
    }
    
    // If all instances belong to one class, create a leaf node
    if (overallCounts.size() == 1) {
        return createLeafNode(data, targetIndex);
    }
    
    // Step 1: Calculate information gain for each attribute
    vector<double> gains = Information_Gain(data, targetIndex);
    
    // Step 2: Find attribute with the highest gain
    int bestAttributeIndex = -1;
    double maxGain = -1;
    for (size_t col = 0; col < gains.size(); ++col) {
        if (gains[col] > maxGain) {
            maxGain = gains[col];
            bestAttributeIndex = col;
        }
    }
    
    // Step 3: Split data into subsets based on best attribute
    map<string, vector<vector<string>>> subsets;
    for (size_t i = 1; i < data.size(); ++i) {
        string attributeValue = data[i][bestAttributeIndex];
        subsets[attributeValue].push_back(data[i]);
    }
    
    // Step 4: Create node
    Node* node = new Node(bestAttributeIndex);
    
    // Step 5: Recursively build children for each subset
    for (const auto& pair : subsets) {
        const string& value = pair.first;
        const vector<vector<string>>& subsetData = pair.second;
        
        Node* childNode = buildTree(subsetData, targetIndex);
        
        // Assign child nodes based on the attribute value
        if (node->left == nullptr) {
            node->left = childNode; // First subset becomes left child
        } else {
            node->right = childNode; // Second subset becomes right child
        }
    }
  
    return node;
}

// Function to print the decision tree
void printTree(Node* node, int level = 0) {
      std::vector<std::string> headers = {
        "buying price",
        "maintenance cost",
        "number of doors",
        "number of persons",
        "lug_boot",
        "safety",
        "decision"
    };
    if (node == nullptr) return;
    
    // Print the current node
    if (node->decision != "") {
        cout << setw(level * 4) << "" << "Decision: " << node->decision << endl;
    } else {
        cout << setw(level * 4) << "" << "Attribute: " << headers[node->attributeIndex] << endl;
        cout << setw(level * 4) << "" << "Left ->" << endl;
        printTree(node->left,  level + 1);
        cout << setw(level * 4) << "" << "Right ->" << endl;
        printTree(node->right, level + 1);
    }
}

// Example usage
int main() {
    int choice = 0;  // Initialize the choice variable
    
    string f_name;
    cout << "Enter Your Training File Name: ";
    cin >> f_name;
    Input train_file(f_name);  // Object to load training data
    int targetIndex = 6;  
    
    // Build the decision tree using the training data
    Node* decisionTree = buildTree(train_file.GetTable(), targetIndex);
    
    do {
        // Ask the user to choose an option
        cout << "Choose an option:\n";
        cout << "1. Print the decision tree\n";
        cout << "2. Verify predictions using test data\n";
        cout << "3. Show training data table\n";
        cout << "4. Show a specific row of the table\n";
        cout << "5. Count unique attributes\n";
        cout << "6. Calculate Information Gain\n";
        cout << "7. Calculate Entropy\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                printTree(decisionTree);
                break;
            case 2:
                {
                    string test_file_name;
                    cout << "Enter Your Test File Name: ";
                    cin >> test_file_name;
                    Input test_file(test_file_name);  // Object to load test data
                    bool allMatched = verifyPrediction(decisionTree, test_file.GetTable(), targetIndex);
                    if (allMatched) {
                        cout << "All predictions matched the actual values." << endl;
                    } else {
                        cout << "Some predictions did not match the actual values." << endl;
                    }
                }
                break;
            case 3:
                {
                    vector<vector<string>> table = train_file.GetTable();
                    for (const auto& row : table) {
                        for (const auto& col : row) {
                            cout << col << " ";
                        }
                        cout << endl;
                    }
                }
                break;
            case 4:
                {
                    int rowIndex;
                    cout << "Enter Row Number: ";
                    cin >> rowIndex;
                    vector<vector<string>> table = train_file.GetTable();
                    if (rowIndex < table.size()) {
                        for (const auto& col : table[rowIndex]) {
                            cout << col << " ";
                        }
                        cout << endl;
                    } else {
                        cout << "Invalid row index." << endl;
                    }
                }
                break;
            case 5:
                CountUniqueAttributes(train_file.GetTable());
                break;
            case 6:
                {
                    vector<double> IG = Information_Gain(train_file.GetTable(), targetIndex);
                    for (auto i = 0; i < IG.size(); i++) {
                        cout << "Information Gain of column " << i << ": " << IG[i] << endl;
                    }
                }
                break;
            case 7:
                {
                    double EN = CalculateEntropy(train_file.GetTable(), targetIndex);
                    cout << "Entropy of Root: " << EN << endl;
                }
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "\nINVALID OPTION, CHOOSE AGAIN\n";
                Sleep(2000);
        }   
        
        // Clear input buffer
        cout << "Press Enter For Next Query: ";
        cin.ignore();  // Clear the input buffer
        cin.get();     // Wait for user input before continuing
        
    } while (choice != 0);
   
    return 0;
}
