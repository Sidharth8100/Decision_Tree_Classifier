#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include<map>
#include<cmath>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Function Declarations
void PrintData(const vector<vector<string>> &table);
void CountUniqueAttributes(const vector<vector<string>> &table);
double getStatisticalError( double f, int N);

/////////////////////////////////////////////////////////////////////////////
class Node {
    int Index;
    string feature_name ;
    Node * left ;
    Node * right ;
    string result ;
    public :
    Node (int index , Node * left = nullptr , Node* right = nullptr )
    {
        Index = index ;
        this->left = left ;
        this->right = right ;

    }
};
class DecisionTree
{
    Node * root = nullptr;
};
class Input
{
    vector<vector<string>> table;
    int T_row = 0;

public:
    Input(string f_name)
    {
        ifstream reader(f_name);
        if (reader.is_open())
        {
            table.resize(1000);
            while (reader.good())
            {
                table[T_row].resize(7);
                for (int i = 0; i < 7; ++i)
                {
                    getline(reader, table[T_row][i], (i < 7 ? ',' : '\n'));
                }
                T_row++;
            }
            if (table.size() > T_row) 
            {
                table.erase(table.begin() + T_row, table.end());
            }
        }
        else
        {
            cout << "File does not exist ;" << endl;
        }
    }
    
    int GetRow() const
    {
        return T_row;
    }

    vector<vector<string>> GetTable() const
    {
        return table;
    }
};
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

void PrintData(const vector<vector<string>> &table)
{
    for (const auto& row : table)
    {
        for (const auto& elem : row)
        {
            cout << elem << ',';
        }
        cout << endl;
    }
}

void CountUniqueAttributes(const vector<vector<string>> &table)  /////meaning it won’t modify the data and avoids copying for efficiency.
{
    if (table.empty()) return;

    // Use a set for each column to track unique attributes
    vector<set<string>> uniqueAttributes(6);
    cout <<endl << "error " << endl ;
    for (int col = 0 ; col < 6 ; col++)
    {   
        for (int row = 0 ; row < table.size() ; row++)
        {     //  cout << row << ' ' << table[row][col] << ':' << ' ';

            uniqueAttributes[col].insert(table[row][col]);
        }
    }
        // Print unique counts for each column
    for (size_t col = 0; col < uniqueAttributes.size(); ++col)
    {
        cout << "Column " << col + 1 << " has " << uniqueAttributes[col].size() << " unique attributes." << endl;
    }
}

double getStatisticalError( double f, int N){
// f -> frequency of incorrect classifications
// N -> number of samples or instances in the node
    double z= 1.96 //95% C.I.
    if(N==0) {
				cout << "Error Located in function for statistical error" << endl;
                cout << "N is zero, ensure N is a valid number"<<endl;
				exit(0);
			}
    return (f+z*z/(2*N)+z*sqrt(f/N-f*f/N+z*z/(4*N*N)))/(1+z*z/N);
}

int main(int argc, const char *argv[])
{
    cout << "argc value : " << argc << endl;
    cout << "argv[0] : " << argv[0] << endl;

    string f_name;
    cout << "Enter Your File Name" << endl;
    cin >> f_name;

    Input train_file(f_name);
    cout << "get Row : " << train_file.GetRow() << endl;
    
    PrintData(train_file.GetTable());
    CountUniqueAttributes(train_file.GetTable());
    double entropy = CalculateEntropy(train_file.GetTable() ,6);
    cout << "BaseEntropy : " << entropy << endl ;
    return 0;
}
