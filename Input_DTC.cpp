#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Function Declarations
void PrintData(const vector<vector<string>> &table);
void CountUniqueAttributes(const vector<vector<string>> &table);

/////////////////////////////////////////////////////////////////////////////

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
                table[T_row].resize(6);
                for (int i = 0; i < 6; ++i)
                {
                    getline(reader, table[T_row][i], (i < 5 ? ',' : '\n'));
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
        for (int row = 0 ; row < table[row].size() ; row++)
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

    return 0;
}
