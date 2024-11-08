#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

class MatrixCls
{
  private:
    vector<vector<string>> Matrix;

  public:
    MatrixCls::MatrixCls(string Data_File) 
{
  Matrix.clear();
  ifstream Data(Data_File);
  string Line, Item;
  vector<string> Row;

  while (getline(Data, Line)) 
  {
    istringstream Iss(Line);
    while (getline(Iss, Item, ',')) 
    {
      // Removing leading and trailing whitespaces
      Item.erase(0, Item.find_first_not_of(" "));
      Item.erase(Item.find_last_not_of(" ") + 1);
      Row.push_back(Item);
    }
    if (!Row.empty()) 
    {
      Matrix.push_back(Row);
      Row.clear();
    }
  }
  Data.close();
}

    string Element(int i, int j) { return Matrix[i][j]; }

    int SizeX() { return Matrix[0].size(); }

    int SizeY() { return Matrix.size(); }

    vector<string> GetVarKinds()
    {
      vector<string> Kinds;
      for (int j = 0; j < SizeX() - 1; j++)
      {
        Kinds.push_back(Matrix[0][j]);
      }
      return Kinds;
    }

    vector<string> GetAttributes()
    {
      vector<string> Attributes;
      for (int j = 0; j < SizeX() - 1; j++)
      {
        Attributes.push_back(Matrix[1][j]);
      }
      return Attributes;
    }

    vector<string> GetScores()
    {
      vector<string> Scores;
      for (int i = 2; i < SizeY(); i++)
      {
        Scores.push_back(Matrix[i][SizeX() - 1]);
      }
      return Scores;
    }

    int GetAttributeIndex(string The_Attribute)
    {
      for (int j = 0; j < SizeX(); j++)
      {
        if (Matrix[1][j] == The_Attribute)
        {
          return j;
        }
      }
      return -1; // If attribute not found
    }

    vector<string> GetAttributeValues(string The_Attribute)
    {
      vector<string> Values;
      int Index = GetAttributeIndex(The_Attribute);
      for (int i = 2; i < SizeY(); i++)
      {
        Values.push_back(Matrix[i][Index]);
      }
      return Values;
    }

    vector<string> GetUniqueAttributeValues(string The_Attribute)
    {
      vector<string> Values = GetAttributeValues(The_Attribute);
      sort(Values.begin(), Values.end());
      Values.erase(unique(Values.begin(), Values.end()), Values.end());
      return Values;
    }

    map<string, vector<string>> GetAttributeValuesScores(string The_Attribute)
    {
      int Index = GetAttributeIndex(The_Attribute);
      map<string, vector<string>> Attribute_Values_Scores;
      vector<string> Attribute_Values = GetUniqueAttributeValues(The_Attribute);
      vector<string> Row;

      for (const string &val : Attribute_Values)
      {
        for (int i = 2; i < SizeY(); i++)
        {
          if (Matrix[i][Index] == val)
          {
            Row.push_back(Matrix[i][SizeX() - 1]);
          }
        }
        Attribute_Values_Scores[val] = Row;
        Row.clear();
      }
      return Attribute_Values_Scores;
    }


    vector<string> SortAttributeValues(string The_Attribute) 
{ 
  vector<string> Values = GetAttributeValues(The_Attribute);  // Retrieve attribute values
  string Temp;

  // Sort the values in ascending order
  for(int i = 0; i < Values.size()-1; i++) 
  { 
    for(int j = i+1; j < Values.size(); j++) 
    { 
      // Swap values if they are out of order
      if(stod(Values[i]) - stod(Values[j]) > 1.e-8) 
      { 
        Temp = Values[i]; 
        Values[i] = Values[j]; 
        Values[j] = Temp; 
      } 
    } 
  } 

  return Values;  // Return the sorted values
} 
vector<string> SortScoreValues(string The_Attribute) {
        vector<string> Values = GetAttributeValues(The_Attribute);  // Retrieve attribute values
        vector<string> Scores = GetScores();  // Retrieve associated scores
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


  
 

    vector<string> GetBisectNodes(string The_Attribute) 
{ 
  vector<string> Bisect_Nodes; 
  vector<string> SortedValues = SortAttributeValues(The_Attribute); 
  vector<string> SortedScores = SortScoreValues(The_Attribute); 
  for(int i = 0; i < SortedValues.size()-1; i++) 
  { 
    if(abs(stod(SortedValues[i]) - stod(SortedValues[i+1])) > 1.e-8 & SortedScores[i].compare(SortedScores[i+1]) != 0) 
    { 
      Bisect_Nodes.push_back(to_string((stod(SortedValues[i]) + stod(SortedValues[i+1]))/2.)); 
    } 
  } 
  return Bisect_Nodes; 
} 


   map<string, vector<string>> GetAttributeBisectParts(string The_Attribute, string Bisect_Node) 
{ 
  map<string, vector<string>> Bisect_Parts;  // Initialize map to store bisected parts
  vector<string> SortedValues = SortAttributeValues(The_Attribute); 
  vector<string> SortedScores = SortScoreValues(The_Attribute); 
  vector<string> Row_1, Row_2, Row_3, Row_4; 
  
  for(int i = 0; i < SortedValues.size(); i++) 
  { 
    // Check if current value is less than the bisect node
    if(stod(SortedValues[i]) - stod(Bisect_Node) < -1.e-8) 
    { 
      Row_1.push_back(SortedScores[i]);  // Add to lower scores
      Row_3.push_back(SortedValues[i]);  // Add to lower values
    }
    else
    { 
      Row_2.push_back(SortedScores[i]);  // Add to upper scores
      Row_4.push_back(SortedValues[i]);  // Add to upper values
    } 
  } 
  
  // Populate the map with categorized vectors
  Bisect_Parts["Lower_Scores"] = Row_1; 
  Bisect_Parts["Upper_Scores"] = Row_2; 
  Bisect_Parts["Lower_Values"] = Row_3; 
  Bisect_Parts["Upper_Values"] = Row_4; 

  return Bisect_Parts; 
} 


    MatrixCls operator()(MatrixCls A_Matrix, string The_Attribute, string The_Value, string Bisect_Node = "")
    {
      Matrix.erase(Matrix.begin(),Matrix.end());
      int i, j;
      int Index = A_Matrix.GetAttributeIndex(The_Attribute);
      vector < string > Kinds = A_Matrix.GetVarKinds();
      vector < string > Row;

      if(Kinds[Index].compare("Discrete") == 0)
      {
        for(i = 0; i < 2; i++)
        {
          for(j = 0; j < A_Matrix.SizeX(); j++)
          {
            if(A_Matrix.Element(1,j).compare(The_Attribute) != 0)
            {
              Row.push_back(A_Matrix.Element(i,j));
            }
          }
          if(Row.size() != 0)
          {
            Matrix.push_back(Row);
            Row.erase(Row.begin(),Row.end());
          }
        }

        for(i = 2; i < A_Matrix.SizeY(); i++)
        {
          for(j = 0; j < A_Matrix.SizeX(); j++)
          {
            if(A_Matrix.Element(1,j).compare(The_Attribute) != 0 & A_Matrix.Element(i,Index).compare(The_Value) == 0)
            {
              Row.push_back(A_Matrix.Element(i,j));
            }
          }

          if(Row.size() != 0)
          {
            Matrix.push_back(Row);
            Row.erase(Row.begin(),Row.end());
          }
        }
        return *this;
      }

      else if(Kinds[Index].compare("Continuous") == 0)
      {
        for(i = 0; i < 2; i++)
        {
          for(j = 0; j < A_Matrix.SizeX(); j++)
          {
            Row.push_back(A_Matrix.Element(i,j));
          }
          if(Row.size() != 0)
          {
            Matrix.push_back(Row);
            Row.erase(Row.begin(),Row.end());
          }
        }

        if(The_Value.compare("Lower_Values") == 0)
        {
          for(i = 2; i < A_Matrix.SizeY(); i++)
          {
            for(j = 0; j < A_Matrix.SizeX(); j++)
            {
              if(stod(A_Matrix.Element(i,Index)) -stod(Bisect_Node) < -1.e-8)
              {
                Row.push_back(A_Matrix.Element(i,j));
              }
            }

            if(Row.size() != 0)
            {
              Matrix.push_back(Row);
              Row.erase(Row.begin(),Row.end());
            }
          }
        }
        else if(The_Value.compare("Upper_Values") == 0)
        {
          for(i = 2; i < A_Matrix.SizeY(); i++)
          {
            for(j = 0; j < A_Matrix.SizeX(); j++)
            {
              if(stod(A_Matrix.Element(i,Index)) - stod(Bisect_Node) > 1.e-8)
              {
                Row.push_back(A_Matrix.Element(i,j));
              }
            }

            if(Row.size() != 0)
            {
              Matrix.push_back(Row);
              Row.erase(Row.begin(),Row.end());
            }
          }
        }
        return *this;
      }
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
    

vector < string > UniqueValues(vector < string > A_String)
{
  sort(A_String.begin(), A_String.end());
	A_String.erase(unique(A_String.begin(), A_String.end()), A_String.end());
  return A_String;
}

string FrequentValues(vector < string > A_String)
{
  vector < string > Unique_Values = UniqueValues(A_String);
  int Count[Unique_Values.size()] = {0};
  for(int i = 0; i < A_String.size(); i++)
  {
    for(int j = 0; j < Unique_Values.size(); j++)
    {
      if(A_String[i].compare(Unique_Values[j]) == 0)
      {
        Count[j] = Count[j] + 1;
      }
    }
  }

  int Max_Count = 0, Max_Index;
  for(int i = 0; i < Unique_Values.size(); i++)
  {
    if(Count[i] > Max_Count)
    {
      Max_Count = Count[i];
      Max_Index = i;
    }
  }
  return Max_Index != -1 ? Unique_Values[Max_Index] : "";
}

double ComputeScoreEntropy(vector < string > Scores)
{
  vector < string > Score_Range = UniqueValues(Scores);
  if(Score_Range.size() == 0)
  {
    return 0.0;
  }
  else
  {
    double TheEntropy = 0.0;
  	int i, j;
  	int Count[Score_Range.size()] = {0};

  	for(i = 0; i < Scores.size(); i++)
  	{
  		for(j = 0; j < Score_Range.size(); j++)
  		{
  			if(Scores[i].compare(Score_Range[j]) == 0)
  			{
  				Count[j] = Count[j] + 1;
  			}
  		}
  	}

  	double Temp_Entropy;
  	double Temp_P;
  	for(j = 0; j < Score_Range.size(); j++)
  	{
  		Temp_P = (double)Count[j]/(double)(Scores.size());
  		Temp_Entropy = -Temp_P*log(Temp_P)/log(2.);
  		TheEntropy = TheEntropy + Temp_Entropy;
  	}
  	return TheEntropy;
  }
}

double ComputeAttributeEntropy(MatrixCls Remain_Matrix, string The_Attribute)
{
  vector < string > Values = Remain_Matrix.GetAttributeValues(The_Attribute);
  return ComputeScoreEntropy(Values);
}

double ComputeAttributeEntropyGain(MatrixCls Remain_Matrix, string The_Attribute, string Bisect_Node = "")
{
  int Index = Remain_Matrix.GetAttributeIndex(The_Attribute);
  vector < string > Kinds = Remain_Matrix.GetVarKinds();
  double Original_Entropy = 0., Gained_Entropy = 0.;
  vector < string > Scores = Remain_Matrix.GetScores();
  Original_Entropy = ComputeScoreEntropy(Scores);

  if(Kinds[Index].compare("Discrete") == 0)
  {
    map < string, vector < string > > Values_Scores = Remain_Matrix.GetAttributeValuesScores(The_Attribute);
    vector < string > Values = Remain_Matrix.GetUniqueAttributeValues(The_Attribute);

  	double After_Entropy = 0.;
  	double Temp_Entropy;
  	vector < string > Temp_Scores;
  	int i,j;
  	for(i = 0; i < Values.size(); i++)
  	{
  		Temp_Scores = Values_Scores[Values[i]];
  		Temp_Entropy = ComputeScoreEntropy(Temp_Scores)*(double)Temp_Scores.size()/(double)Scores.size();
  		After_Entropy = After_Entropy + Temp_Entropy;
  	}
  	Gained_Entropy = Original_Entropy -  After_Entropy;
  	return Gained_Entropy;
  }

  if(Kinds[Index].compare("Continuous") == 0)
  {
    map < string, vector < string > > Parts = Remain_Matrix.GetAttributeBisectParts(The_Attribute,Bisect_Node);
    double LowerLen = Parts["Lower_Scores"].size();
    double UpperLen = Parts["Upper_Scores"].size();
    double Len = LowerLen + UpperLen;
    double After_Entropy, Gained_Entropy;
    After_Entropy = LowerLen/Len*ComputeScoreEntropy(Parts["Lower_Scores"]) + UpperLen/Len*ComputeScoreEntropy(Parts["Upper_Scores"]);
    Gained_Entropy = Original_Entropy - After_Entropy;
    return Gained_Entropy;
  }
}

double GainRatio(MatrixCls Remain_Matrix, string The_Attribute, string Bisect_Node = "") {
    double Attribute_Entropy = ComputeAttributeEntropy(Remain_Matrix, The_Attribute);
    double Attribute_Entropy_Gain = ComputeAttributeEntropyGain(Remain_Matrix, The_Attribute, Bisect_Node);
    return Attribute_Entropy > 0 ? Attribute_Entropy_Gain / Attribute_Entropy : 0.0; // Prevent division by zero
}

class TreeCls
{
  public:
    string Node;
    string Branch;
    vector < TreeCls * > Child;
    TreeCls()TreeCls() : Node(""), Branch("") {}

    TreeCls * BuildTree(TreeCls * Tree, MatrixCls Remain_Matrix);
    void Display(int Depth);
    string Temp_TestTree(vector < string > Kinds, vector < string > Attributes, vector < string > Value, vector < string > Score_Range);
    vector < string > TestTree(MatrixCls Data_Matrix);
};

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

vector < string > TreeCls::TestTree(MatrixCls Data_Matrix)
{

  int Lines_Number = Data_Matrix.SizeY() - 2;
  vector < string > Test_Scores;
  int i, j, k, l, m;
  vector < string > Kinds = Data_Matrix.GetVarKinds();
  vector < string > Attributes = Data_Matrix.GetAttributes();
  vector < string > Attributes_Value;
  vector < string > Score_Range = UniqueValues(Data_Matrix.GetScores());

  for(i = 0; i < Lines_Number; i++)
  {

    for(j = 0; j < Data_Matrix.SizeX() - 1; j++)
    {
      Attributes_Value.push_back(Data_Matrix.Element((i+2),j));
    }

    string Temp_Score;
    Temp_Score = Temp_TestTree(Kinds, Attributes, Attributes_Value, Score_Range);
    Test_Scores.push_back(Temp_Score);
    Attributes_Value.erase(Attributes_Value.begin(),Attributes_Value.end());
  }
  return Test_Scores;
}



TreeCls * TreeCls::BuildTree(TreeCls * Tree, MatrixCls Remain_Matrix)
{
  if (Tree == nullptr)
  {
    Tree = new TreeCls();
  }

  // Get unique score values from the remaining matrix
  vector<string> Score_Range = UniqueValues(Remain_Matrix.GetScores());
  
  // If there's only one score value left, make it the current node
  if (Score_Range.size() == 1)
  {
    Tree->Node = Score_Range[0];
    return Tree;
  }

  // Get the types of attributes in the matrix
  vector<string> Kinds = Remain_Matrix.GetVarKinds();

  double Max_Gain_Ratio = -1.0;
  string Best_Attribute;
  string Best_Bisect_Node;
  
  // Search for the best attribute based on gain ratio
  for (int i = 0; i < Kinds.size(); i++)
  {
    string The_Attribute = Remain_Matrix.GetAttributes()[i];
    double Gain_Ratio_Value;
    
    if (Kinds[i] == "Discrete")
    {
      Gain_Ratio_Value = GainRatio(Remain_Matrix, The_Attribute);
    }
    else if (Kinds[i] == "Continuous")
    {
      vector<string> Bisect_Nodes = Remain_Matrix.GetBisectNodes(The_Attribute);
      for (string Bisect_Node : Bisect_Nodes)
      {
        double Temp_Gain_Ratio = GainRatio(Remain_Matrix, The_Attribute, Bisect_Node);
        if (Temp_Gain_Ratio > Gain_Ratio_Value)
        {
          Gain_Ratio_Value = Temp_Gain_Ratio;
          Best_Bisect_Node = Bisect_Node;
        }
      }
    }
    
    if (Gain_Ratio_Value > Max_Gain_Ratio)
    {
      Max_Gain_Ratio = Gain_Ratio_Value;
      Best_Attribute = The_Attribute;
    }
  }

  Tree->Node = Best_Attribute;

  if (Kinds[Remain_Matrix.GetAttributeIndex(Best_Attribute)] == "Discrete")
  {
    vector<string> Unique_Values = Remain_Matrix.GetUniqueAttributeValues(Best_Attribute);
    for (string Value : Unique_Values)
    {
      TreeCls * Subtree = new TreeCls();
      Subtree->Branch = Value;
      MatrixCls Submatrix = Remain_Matrix(Remain_Matrix, Best_Attribute, Value);
      Tree->Child.push_back(BuildTree(Subtree, Submatrix));
    }
  }
  else if (Kinds[Remain_Matrix.GetAttributeIndex(Best_Attribute)] == "Continuous")
  {
    TreeCls * Lower_Subtree = new TreeCls();
    Lower_Subtree->Branch = "< " + Best_Bisect_Node;
    MatrixCls Lower_Submatrix = Remain_Matrix(Remain_Matrix, Best_Attribute, "Lower_Values", Best_Bisect_Node);
    Tree->Child.push_back(BuildTree(Lower_Subtree, Lower_Submatrix));

    TreeCls * Upper_Subtree = new TreeCls();
    Upper_Subtree->Branch = ">= " + Best_Bisect_Node;
    MatrixCls Upper_Submatrix = Remain_Matrix(Remain_Matrix, Best_Attribute, "Upper_Values", Best_Bisect_Node);
    Tree->Child.push_back(BuildTree(Upper_Subtree, Upper_Submatrix));
  }

  return Tree;
}

void TreeCls::Display(int Depth)
{
  for (int i = 0; i < Depth; i++)
  {
    cout << "\t";
  }
  cout << Node << endl;
  for (TreeCls * Child_Tree : Child)
  {
    for (int i = 0; i < Depth + 1; i++)
    {
      cout << "\t";
    }
    cout << "|-- " << Child_Tree->Branch << endl;
    Child_Tree->Display(Depth + 1);
  }
}


  vector < string > Unique_Scores = UniqueValues(Remain_Matrix.GetScores());
  if(Unique_Scores.size() == 1)
  {
    Tree->Node = Unique_Scores[0];
    return Tree;
  }

  //vector < string > Scores = Remain_Matrix.GetScores();
  //if(Scores.size() <= 5)
  //{
  //  Tree->Node = FrequentValues(Scores);
  //  return Tree;
  //}

  double Gain_Ratio = 0, Entropy_Gain = 0;
  double Temp_Gain_Ratio, Temp_Entropy_Gain;
  string Max_Attribute;
  string Max_Bisect_Node = "";
  int Max_Attribute_Index = 0;
  //string Max_Bisect_Node_Index = 0;
  vector < string > Attributes = Remain_Matrix.GetAttributes();
  vector < string > Kinds = Remain_Matrix.GetVarKinds();
  int i, j;
  for(i = 0; i < Attributes.size(); i++)
  {
    if(Kinds[i].compare("Discrete") == 0)
    {
      Temp_Gain_Ratio = GainRatio(Remain_Matrix,Attributes[i]);
    }
    else if(Kinds[i].compare("Continuous") == 0)
    {
      vector < string > Bisect_Nodes = Remain_Matrix.GetBisectNodes(Attributes[i]);
      for(j = 0; j < Bisect_Nodes.size(); j++)
      {
        Temp_Entropy_Gain = ComputeAttributeEntropyGain(Remain_Matrix,Attributes[i],Bisect_Nodes[j]);
        if(Temp_Entropy_Gain - Entropy_Gain > 1.e-8 )
        {
          Entropy_Gain = Temp_Entropy_Gain;
          Max_Bisect_Node = Bisect_Nodes[j];
        }
      }
      Temp_Gain_Ratio = GainRatio(Remain_Matrix,Attributes[i], Max_Bisect_Node);
    }

    if(Temp_Gain_Ratio - Gain_Ratio > 1.e-8)
    {
      Gain_Ratio = Temp_Gain_Ratio;
      Max_Attribute = Attributes[i];
      Max_Attribute_Index = i;
    }
  }

  Tree->Node = Max_Attribute;
  vector < string > Values, Branch_Values;
  if(Kinds[Max_Attribute_Index].compare("Discrete") == 0)
  {
    Values = Remain_Matrix.GetUniqueAttributeValues(Max_Attribute);
    Branch_Values = Values;
  }
  else if(Kinds[Max_Attribute_Index].compare("Continuous") == 0)
  {
    Values = {"Lower_Values", "Upper_Values"};
    string Left_Branch = "< " + Max_Bisect_Node;
    string Right_Branch = "> " + Max_Bisect_Node;
    Branch_Values = {Left_Branch, Right_Branch};
  }

  int k;
  MatrixCls New_Matrix;
  for(k = 0; k < Values.size(); k++)
  {
    New_Matrix = New_Matrix.operator()(Remain_Matrix, Max_Attribute, Values[k], Max_Bisect_Node);
    TreeCls * New_Tree = new TreeCls();
    New_Tree->Branch = Branch_Values[k];
    vector < string > New_Unique_Scores = UniqueValues(New_Matrix.GetScores());
    if(New_Unique_Scores.size() == 1)
    {
      New_Tree->Node = New_Unique_Scores[0];
    }
    else
    {
      BuildTree(New_Tree, New_Matrix);
    }
    Tree->Child.push_back(New_Tree);
  }
  return Tree;
}

void TreeCls::Display(int Depth = 0)
{
  for(int i = 0; i < Depth; i++)
  {
    cout << "\t";
  }
  if(this->Branch.compare("") != 0)
  {
    cout << this->Branch << endl;
    for(int i = 0; i < Depth+1; i++)
    {
      cout << "\t";
    }
  }
  cout << this->Node << endl;
  for(int i = 0; i < this->Child.size(); i++)
  {
    (this->Child[i])->Display(Depth + 1);
  }
}

void DisplayVector(vector < string > The_Vector)
{
  for(int i = 0; i < The_Vector.size(); i++)
  {
    cout << The_Vector[i] << "\n" ;
  }
  cout << endl;
}

int main()
{
  MatrixCls Matrix("Golf.dat");
  TreeCls * Tree;
  Tree = Tree->BuildTree(Tree, Matrix);
  Tree->Display();
  vector < string > Test_Scores = Tree->TestTree(Matrix);
  DisplayVector(Test_Scores);
  vector < string > Test_String = {"a","b","c","d"};
  DisplayVector(Test_String);
} 
