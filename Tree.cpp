#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <iomanip>

///////////////////////

#include"Information_entropy.cpp"
#include"Input_dtc.cpp"
#include"EntropyCal.cpp"

//////////////////////
using namespace std;

// Structure for a tree node
struct Node {
    int attributeIndex; // Index of the attribute for the decision
    Node* left; // Left child
    Node* right; // Right child
    string decision; // Decision at leaf node (Yes/No)

    Node(int index) : attributeIndex(index), left(nullptr), right(nullptr), decision("") {}
};

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
void printTree(Node* node, const vector<string>& headers, int level = 0) {
    if (node == nullptr) return;

    // Print the current node
    if (node->decision != "") {
        cout << setw(level * 4) << "" << "Decision: " << node->decision << endl;
    } else {
        cout << setw(level * 4) << "" << "Attribute: " << headers[node->attributeIndex] << endl;
        cout << setw(level * 4) << "" << "Left ->" << endl;
        printTree(node->left, headers, level + 1);
        cout << setw(level * 4) << "" << "Right ->" << endl;
        printTree(node->right, headers, level + 1);
    }
}

// Example usage
int main() {
    string f_name;
    cout << "Enter Your File Name" << endl;
    cin >> f_name;
    
    Input train_file(f_name);           //// OBEJCT of Data file
    int targetIndex = 6;    

    Node* decisionTree = buildTree(train_file.GetTable(), targetIndex);

    // Print the decision tree
    printTree(decisionTree, train_file.GetTable()[0]);

    // Code to free memory would go here (not shown for brevity)

    return 0;
}
