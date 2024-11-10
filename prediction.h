#ifndef PREDICT_H
#define PREDICT_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Node {
    int attributeIndex;
    Node* left;
    Node* right;
    string decision;

    Node(int index)
        : attributeIndex(index),
          left(nullptr),
          right(nullptr),
          decision("") 
    {
    }
};

string predict(Node* node, const vector<string>& row, const vector<string>& headers) 
{
    while (node->decision == "") 
    {
        int attributeIdx = node->attributeIndex;
        string attributeValue = row[attributeIdx];

        if (node->left) 
        {
            if (attributeValue == node->left->decision) 
            {
                node = node->left;
                continue;
            }
        }

        if (node->right) 
        {
            node = node->right;
        } 
        else 
        {
            return "Unknown";
        }
    }
    
    string finalDecision;
    finalDecision = node->decision;
    
    return finalDecision;
}

bool verifyPrediction(Node* root, const vector<vector<string>>& table, int targetIndex) 
{
    bool allMatched = true;
    double match=0 ;
    for (size_t rowIdx = 1; rowIdx < table.size(); ++rowIdx) 
    {
        const vector<string>& row = table[rowIdx];
        string actualValue = row[targetIndex];
        string predictedValue = predict(root, row, table[0]);

        if (predictedValue == actualValue) 
        {
            cout << "Row " << rowIdx << " - Match: ";
            // for (const string& element : row) 
            // {
            //     cout << element << " ";
            // }
            cout << endl;match++;
        } 
        else 
        {
            allMatched = false;
            cout << "Row " << rowIdx << " - Mismatch: ";
            // for (const string& element : row) 
            // {
            //     cout << element << " ";
            // }
            cout << endl;
        }
    }
    int size = table.size() ;
    double percent = match /size  ;
    cout << match <<' '<< size << ' '<< percent << endl ;
    return allMatched;
}

#endif 
