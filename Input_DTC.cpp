#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Function Declaration
void PrintData(vector<vector<string>> &table);

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
            string B_Price, Maint_cost, N_doors, N_person, Lug_capacity, Safety;
            table.resize(1000);

            while (reader.good())
            {
                table[T_row].resize(6);
                getline(reader, table[T_row][0], ',');
                getline(reader, table[T_row][1], ',');
                getline(reader, table[T_row][2], ',');
                getline(reader, table[T_row][3], ',');
                getline(reader, table[T_row][4], ',');
                getline(reader, table[T_row][5], '\n');

                T_row++;
            }  
            if(table.size() > T_row) 
            {
                table.erase(table.begin() + T_row ,table.end() ) ;
            }
        }
        else
        {
            cout << "File does not exist ;" << endl;
        }
    }
    int GetRow()
    {
        return T_row;
    }
    vector<vector<string>>Get_table()
    {
        return table ;
    }
};

void PrintData(vector<vector<string>> &table)
{
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[0].size(); j++)
        {
            cout << table[i][j] << ' ' << ',';
        }
        cout << endl;
    }
}

int main(int argc, const char *argv[])
{
    cout << "argc value : " << argc << endl;
    cout << "argv[0] : " << argv[0] << endl;
    // cout << "argv[1] : "<< argv[1] << endl ;
    string f_name;
    cout << "Enter Your File Name" << endl;
    cin >> f_name;
    Input train_file(f_name) ;
    cout << "get Row : "<<train_file.GetRow() << endl ;
    PrintData(train_file.Get_table()) ;
    

    return 0;
}