#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

class MatrixCls {
private:
    vector<vector<string>> Matrix;

public:
    MatrixCls() {} // Default constructor

    MatrixCls(string Data_File) {
        Matrix.clear();
        ifstream Data(Data_File);
        string Line, Item;
        vector<string> Row;

        while (getline(Data, Line)) {
            istringstream Iss(Line);
            while (getline(Iss, Item, ',')) {
                // Removing leading and trailing whitespaces
                Item.erase(0, Item.find_first_not_of(" "));
                Item.erase(Item.find_last_not_of(" ") + 1);
                Row.push_back(Item);
            }
            if (!Row.empty()) {
                Matrix.push_back(Row);
                Row.clear();
            }
        }
        Data.close();
    }

    string Element(int i, int j) { return Matrix[i][j]; }

    int SizeX() { return Matrix.empty() ? 0 : Matrix[0].size(); }

    int SizeY() { return Matrix.size(); }

    vector<string> GetVarKinds() {
        vector<string> Kinds;
        for (int j = 0; j < SizeX() - 1; j++) {
            Kinds.push_back(Matrix[0][j]);
        }
        return Kinds;
    }

    vector<string> GetAttributes() {
        vector<string> Attributes;
        for (int j = 0; j < SizeX() - 1; j++) {
            Attributes.push_back(Matrix[1][j]);
        }
        return Attributes;
    }

    vector<string> GetScores() {
        vector<string> Scores;
        for (int i = 2; i < SizeY(); i++) {
            Scores.push_back(Matrix[i][SizeX() - 1]);
        }
        return Scores;
    }

    int GetAttributeIndex(string The_Attribute) {
        for (int j = 0; j < SizeX(); j++) {
            if (Matrix[1][j] == The_Attribute) {
                return j;
            }
        }
        return -1; // If attribute not found
    }

    vector<string> GetAttributeValues(string The_Attribute) {
        vector<string> Values;
        int Index = GetAttributeIndex(The_Attribute);
        if (Index == -1) return Values; // Return empty if index not found
        for (int i = 2; i < SizeY(); i++) {
            Values.push_back(Matrix[i][Index]);
        }
        return Values;
    }

    vector<string> GetUniqueAttributeValues(string The_Attribute) {
        vector<string> Values = GetAttributeValues(The_Attribute);
        sort(Values.begin(), Values.end());
        Values.erase(unique(Values.begin(), Values.end()), Values.end());
        return Values;
    }

    map<string, vector<string>> GetAttributeValuesScores(string The_Attribute) {
        int Index = GetAttributeIndex(The_Attribute);
        map<string, vector<string>> Attribute_Values_Scores;
        vector<string> Attribute_Values = GetUniqueAttributeValues(The_Attribute);
        vector<string> Row;

        for (const string &val : Attribute_Values) {
            for (int i = 2; i < SizeY(); i++) {
                if (Matrix[i][Index] == val) {
                    Row.push_back(Matrix[i][SizeX() - 1]);
                }
            }
                        Attribute_Values_Scores[val] = Row;
            Row.clear();
        }
        return Attribute_Values_Scores;
    }

    vector<string> SortAttributeValues(string The_Attribute) {
        vector<string> Values = GetAttributeValues(The_Attribute);
        sort(Values.begin(), Values.end());
        return Values;  // Return the sorted values
    }

    vector<string> SortScoreValues(string The_Attribute) {
        vector<string> Values = GetAttributeValues(The_Attribute);
        vector<string> Scores = GetScores();
        vector<pair<double, string>> ValueScorePairs;

        for (size_t i = 0; i < Values.size(); ++i) {
            ValueScorePairs.emplace_back(stod(Values[i]), Scores[i]);
        }

        sort(ValueScorePairs.begin(), ValueScorePairs.end());

        // Extract sorted scores based on sorted values
        vector<string> SortedScores;
        for (const auto& pair : ValueScorePairs) {
            SortedScores.push_back(pair.second);
        }

        return SortedScores;  // Return the sorted scores
    }

    vector<string> GetBisectNodes(string The_Attribute) {
        vector<string> Bisect_Nodes;
        vector<string> SortedValues = SortAttributeValues(The_Attribute);
        vector<string> SortedScores = SortScoreValues(The_Attribute);
        for (int i = 0; i < SortedValues.size() - 1; i++) {
            if (abs(stod(SortedValues[i]) - stod(SortedValues[i + 1])) > 1.e-8 && 
                SortedScores[i].compare(SortedScores[i + 1]) != 0) {
                Bisect_Nodes.push_back(to_string((stod(SortedValues[i]) + stod(SortedValues[i + 1])) / 2.));
            }
        }
        return Bisect_Nodes;
    }

    map<string, vector<string>> GetAttributeBisectParts(string The_Attribute, string Bisect_Node) {
        map<string, vector<string>> Bisect_Parts;
        vector<string> SortedValues = SortAttributeValues(The_Attribute);
        vector<string> SortedScores = SortScoreValues(The_Attribute);
        vector<string> Row_1, Row_2, Row_3, Row_4;

        for (int i = 0; i < SortedValues.size(); i++) {
            if (stod(SortedValues[i]) - stod(Bisect_Node) < -1.e-8) {
                Row_1.push_back(SortedScores[i]);
                Row_3.push_back(SortedValues[i]);
            } else {
                Row_2.push_back(SortedScores[i]);
                Row_4.push_back(SortedValues[i]);
            }
        }

        Bisect_Parts["Lower_Scores"] = Row_1;
        Bisect_Parts["Upper_Scores"] = Row_2;
        Bisect_Parts["Lower_Values"] = Row_3;
        Bisect_Parts["Upper_Values"] = Row_4;

        return Bisect_Parts;
    }

    MatrixCls operator()(MatrixCls A_Matrix, string The_Attribute, string The_Value, string Bisect_Node = "") {
        Matrix.clear();
        int Index = A_Matrix.GetAttributeIndex(The_Attribute);
        vector<string> Kinds = A_Matrix.GetVarKinds();
        vector<string> Row;

        if (Index == -1) return *this; // Return empty if attribute not found

        if (Kinds[Index].compare("Discrete") == 0) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < A_Matrix.SizeX(); j++) {
                    if (A_Matrix.Element(1, j).compare(The_Attribute) != 0) {
                        Row.push_back(A_Matrix.Element(i, j));
                    }
                }
                if (!Row.empty()) {
                    Matrix.push_back(Row);
                    Row.clear();
                }
            }

            for (int i = 2; i < A_Matrix.SizeY(); i++) {
                for (int j = 0; j < A_Matrix.SizeX(); j++) {
                    if (A_Matrix.Element(1, j).compare(The_Attribute) != 0 && 
                        A_Matrix.Element(i, Index).compare(The_Value) == 0) {
                        Row.push_back(A_Matrix.Element(i, j));
                    }
                }

                if (!Row.empty()) {
                    Matrix.push_back(Row);
                    Row.clear();
                }
            }
            return *this;
        } else if (Kinds[Index].compare("Continuous") == 0) {
                        for (int i = 0; i < 2; i++) {
                for (int j = 0; j < A_Matrix.SizeX(); j++) {
                    Row.push_back(A_Matrix.Element(i, j));
                }
                if (!Row.empty()) {
                    Matrix.push_back(Row);
                    Row.clear();
                }
            }

            if (The_Value.compare("Lower_Values") == 0) {
                for (int i = 2; i < A_Matrix.SizeY(); i++) {
                    for (int j = 0; j < A_Matrix.SizeX(); j++) {
                        if (stod(A_Matrix.Element(i, Index)) - stod(Bisect_Node) < -1.e-8) {
                            Row.push_back(A_Matrix.Element(i, j));
                        }
                    }

                    if (!Row.empty()) {
                        Matrix.push_back(Row);
                        Row.clear();
                    }
                }
            } else if (The_Value.compare("Upper_Values") == 0) {
                for (int i = 2; i < A_Matrix.SizeY(); i++) {
                    for (int j = 0; j < A_Matrix.SizeX(); j++) {
                        if (stod(A_Matrix.Element(i, Index)) - stod(Bisect_Node) > 1.e-8) {
                            Row.push_back(A_Matrix.Element(i, j));
                        }
                    }

                    if (!Row.empty()) {
                        Matrix.push_back(Row);
                        Row.clear();
                    }
                }
            }
            return *this;
        }
        return *this; // Return the current instance if no conditions are met
    }

    void Display() {
        for (const auto& row : Matrix) {
            for (const auto& elem : row) {
                cout << elem << "\t";
            }
            cout << endl;
        }
    }
};

// Function to get unique values from a vector of strings
vector<string> UniqueValues(vector<string> A_String) {
    sort(A_String.begin(), A_String.end());
    A_String.erase(unique(A_String.begin(), A_String.end()), A_String.end());
    return A_String;
}

// Function to find the most frequent value in a vector of strings
string FrequentValues(vector<string> A_String) {
    vector<string> Unique_Values = UniqueValues(A_String);
    vector<int> Count(Unique_Values.size(), 0);
    
    for (const auto& value : A_String) {
        auto it = find(Unique_Values.begin(), Unique_Values.end(), value);
        if (it != Unique_Values.end()) {
            Count[it - Unique_Values.begin()]++;
        }
    }

    int Max_Count = 0, Max_Index = -1;
    for (int i = 0; i < Unique_Values.size(); i++) {
        if (Count[i] > Max_Count) {
            Max_Count = Count[i];
            Max_Index = i;
        }
    }
    return Max_Index != -1 ? Unique_Values[Max_Index] : "";
}

// Function to compute the entropy of a set of scores
double ComputeScoreEntropy(vector<string> Scores) {
    vector<string> Score_Range = UniqueValues(Scores);
    if (Score_Range.empty()) {
        return 0.0;
    } else {
        double TheEntropy = 0.0;
        vector<int> Count(Score_Range.size(), 0);

        for (const auto& score : Scores) {
            auto it = find(Score_Range.begin(), Score_Range.end(), score);
            if (it != Score_Range.end()) {
                Count[it - Score_Range.begin()]++;
            }
        }

        for (int j = 0; j < Score_Range.size(); j++) {
            double Temp_P = static_cast<double>(Count[j]) / Scores.size();
            if (Temp_P > 0) {
                TheEntropy -= Temp_P * log(Temp_P) / log(2.0);
            }
        }
        return TheEntropy;
    }
}

// Function to compute the entropy of an attribute in the remaining matrix
double ComputeAttributeEntropy(MatrixCls Remain_Matrix, string The_Attribute) {
    vector<string> Values = Remain_Matrix.GetAttributeValues(The_Attribute);
    return ComputeScoreEntropy(Values);
}

// Function to compute the gain in entropy from splitting on an attribute
double ComputeAttributeEntropyGain(MatrixCls Remain_Matrix, string The_Attribute, string Bisect_Node = "") {
    int Index = Remain_Matrix.GetAttributeIndex(The_Attribute);
    vector<string> Kinds = Remain_Matrix.GetVarKinds();
    double Original_Entropy = ComputeScoreEntropy(Remain_Matrix.GetScores());
    if (Kinds[Index].compare("Discrete") == 0) {
        map<string, vector<string>> Values_Scores = Remain_Matrix.GetAttributeValuesScores(The_Attribute);
        vector<string> Values = Remain_Matrix.GetUniqueAttributeValues(The_Attribute);
        double After_Entropy = 0.0;
        vector<string> Temp_Scores;

        for (const auto& value : Values) {
            Temp_Scores = Values_Scores[value];
            double Temp_Entropy = ComputeScoreEntropy(Temp_Scores) * (static_cast<double>(Temp_Scores.size()) / Remain_Matrix.GetScores().size());
            After_Entropy += Temp_Entropy;
        }
        return Original_Entropy - After_Entropy;
    }

    if (Kinds[Index].compare("Continuous") == 0) {
        map<string, vector<string>> Parts = Remain_Matrix.GetAttributeBisectParts(The_Attribute, Bisect_Node);
        double LowerLen = Parts["Lower_Scores"].size();
        double UpperLen = Parts["Upper_Scores"].size();
        double Len = LowerLen + UpperLen;
        double After_Entropy = (LowerLen / Len * ComputeScoreEntropy(Parts["Lower_Scores"])) +
                               (UpperLen / Len * ComputeScoreEntropy(Parts["Upper_Scores"]));
        return Original_Entropy - After_Entropy;
    }
    return 0.0; // Default return value
}

// Function to compute the gain ratio for an attribute
double GainRatio(MatrixCls Remain_Matrix, string The_Attribute, string Bisect_Node = "") {
    double Attribute_Entropy = ComputeAttributeEntropy(Remain_Matrix, The_Attribute);
    double Attribute_Entropy_Gain = ComputeAttributeEntropyGain(Remain_Matrix, The_Attribute, Bisect_Node);
    return Attribute_Entropy > 0 ? Attribute_Entropy_Gain / Attribute_Entropy : 0.0; // Prevent division by zero
}

class TreeCls {
public:
    string Node;
    string Branch;
    vector<TreeCls*> Child;

    TreeCls() : Node(""), Branch("") {}
    ~TreeCls() {
        for (auto child : Child) {
            delete child; // Free allocated memory for child nodes
        }
    }

    TreeCls* BuildTree(TreeCls* Tree, MatrixCls Remain_Matrix);
    void Display(int Depth = 0);
    string Temp_TestTree(vector<string> Kinds, vector<string> Attributes, vector<string> Value, vector<string> Score_Range);
    vector<string> TestTree(MatrixCls Data_Matrix);
};

// Function to test the tree with a given set of values
string TreeCls::Temp_TestTree(vector<string> Kinds, vector<string> Attributes, vector<string> Value, vector<string> Score_Range) {
    for (const auto& score : Score_Range) {
        if (this->Node == score) {
            return this->Node;
        }
    }

    for (size_t i = 0; i < Attributes.size(); i++) {
        if (this->Node == Attributes[i]) {
            if (Kinds[i] == "Discrete") {
                for (size_t j = 0; j < this->Child.size(); j++) {
                    if (this->Child[j]->Branch == Value[i]) {
                        return this->Child[j]->Temp_TestTree(Kinds, Attributes, Value, Score_Range);
                    }
                }
            } else if (Kinds[i] == "Continuous") {
                string Threshold = this->Child[0]->Branch.substr(2); // Skip "< "
                if (stod(Value[i]) < stod(Threshold)) {
                    return this->Child[0]->Temp_TestTree(Kinds, Attributes, Value, Score_Range);
                } else {
                    return this->Child[1]->Temp_TestTree(Kinds, Attributes, Value, Score_Range);
                }
            }
        }
    }
    return ""; // Default return if not found
}

// Function to test the decision tree with a data matrix
vector<string> TreeCls::TestTree(MatrixCls Data_Matrix) {
    int Lines_Number = Data_Matrix.SizeY() - 2;
    vector<string> Test_Scores;
    vector<string> Kinds = Data_Matrix.GetVarKinds();
    vector<string> Attributes = Data_Matrix.GetAttributes();
    vector<string> Attributes_Value;
    vector<string> Score_Range = UniqueValues(Data_Matrix.GetScores());

    for (int i = 0; i < Lines_Number; i++) {
        Attributes_Value.clear();
        for (int j = 0; j < Data_Matrix.SizeX() - 1; j++) {
            Attributes_Value.push_back(Data_Matrix.Element((i + 2), j));
        }

               string Result = this->Temp_TestTree(Kinds, Attributes, Attributes_Value, Score_Range);
        Test_Scores.push_back(Result);
    }
    return Test_Scores; // Return the list of predicted scores
}

// Function to display the decision tree
void TreeCls::Display(int Depth) {
    for (int i = 0; i < Depth; i++) {
        cout << "  "; // Indentation for depth
    }
    cout << "Node: " << Node << ", Branch: " << Branch << endl;
    for (auto child : Child) {
        child->Display(Depth + 1); // Recursive display for child nodes
    }
}

// Function to build the decision tree
TreeCls* TreeCls::BuildTree(TreeCls* Tree, MatrixCls Remain_Matrix) {
    // Base case: if all scores are the same, return a leaf node
    string Most_Frequent_Score = FrequentValues(Remain_Matrix.GetScores());
    if (Most_Frequent_Score.empty()) {
        return Tree; // No scores available
    }

    // Check if there are no attributes left to split on
    vector<string> Attributes = Remain_Matrix.GetAttributes();
    if (Attributes.empty()) {
        Tree->Node = Most_Frequent_Score; // Assign the most frequent score
        return Tree;
    }

    // Find the best attribute to split on
    double Best_Gain = -1.0;
    string Best_Attribute;
    string Best_Bisect_Node;

    for (const auto& attribute : Attributes) {
        double Gain = GainRatio(Remain_Matrix, attribute);
        if (Gain > Best_Gain) {
            Best_Gain = Gain;
            Best_Attribute = attribute;
            // If the attribute is continuous, find the best bisect node
            if (Remain_Matrix.GetVarKinds()[Remain_Matrix.GetAttributeIndex(attribute)] == "Continuous") {
                vector<string> Bisect_Nodes = Remain_Matrix.GetBisectNodes(attribute);
                if (!Bisect_Nodes.empty()) {
                    Best_Bisect_Node = Bisect_Nodes[0]; // Choose the first bisect node for simplicity
                }
            }
        }
    }

    // Set the node and branch for the current tree node
    Tree->Node = Best_Attribute;
    Tree->Branch = Best_Bisect_Node;

    // Split the remaining matrix based on the best attribute
    MatrixCls Lower_Matrix = Remain_Matrix(Best_Attribute, "Lower_Values", Best_Bisect_Node);
    MatrixCls Upper_Matrix = Remain_Matrix(Best_Attribute, "Upper_Values", Best_Bisect_Node);

    // Create child nodes for the split
    TreeCls* Lower_Child = new TreeCls();
    TreeCls* Upper_Child = new TreeCls();
    Tree->Child.push_back(Lower_Child);
    Tree->Child.push_back(Upper_Child);

    // Recursively build the tree for each child
    Lower_Child->BuildTree(Lower_Child, Lower_Matrix);
    Upper_Child->BuildTree(Upper_Child, Upper_Matrix);

    return Tree; // Return the constructed tree
}

// Main function to demonstrate the decision tree
int main() {
    // Example usage of the MatrixCls and TreeCls
    MatrixCls dataMatrix;
    // Load or initialize dataMatrix with your data here

    TreeCls decisionTree;
    decisionTree.BuildTree(&decisionTree, dataMatrix);

    cout << "Decision Tree Structure:" << endl;
    decisionTree.Display();

    // Test the decision tree with new data
    MatrixCls testMatrix;
    // Load or initialize testMatrix with your test data here

    vector<string> predictions = decisionTree.TestTree(testMatrix);
    cout << "Predictions:" << endl;
    for (const auto& prediction : predictions) {
        cout << prediction << endl;
    }

    return 0; // End of the program
}
