#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
#include<sstream>
#include<algorithm>
#include<functional>

using namespace std;

class Table
{
private:

	typedef struct _column {
		string column_name;
		vector<string> data;
		int max_wide;
	}Column;

	const string UNK{ "UNK" };
	ofstream fout;
	ifstream fin;
	void Print_Line();
	void Get_Sort_temp(vector<int> & temp, vector<string> column_order);
public:
	string File_name;
	string table_name;
	vector<Column> column;
	void Inset(vector<string> order_array);
	void Delete(vector<string> order_array);
	void Update(vector<string> order_array);
	void Select(vector<string> order_array);
	void Get_Filename(string name);
	void File_Write();
	void Print_table();
	void Divide_string(string str,vector<string>& str_array);
	bool Check_column(vector<string> check_help);
	bool Get_index(string check_help, string value_help, vector<int>& index, function<bool(string, string)> cmp);
	void Get_table_help(Table & table_help, vector<string> column_help, vector<int> index);
	void Get_table_help(Table& table_help, vector<string> column_help);
	Table(const Table& origin);
	Table();
	~Table();
};
