#ifndef PREDICT_H
#define PREDICT_H

#include <vector>
#include <string>


struct Node {
    int attributeIndex;
    Node* left;
    Node* right;
    std::string decision;

    Node(int index)
        : attributeIndex(index),
          left(nullptr),
          right(nullptr),
          decision("") 
    {
    }
};

std::string predict(Node* node, 
                    const std::vector<std::string>& row, 
                    const std::vector<std::string>& headers) 
{
    while (node->decision == "") 
    {
        int attributeIdx = node->attributeIndex;
        std::string attributeValue = row[attributeIdx];

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
    
    std::string finalDecision;
    finalDecision = node->decision;
    
    return finalDecision;
}

#endif 
