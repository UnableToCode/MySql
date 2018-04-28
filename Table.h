#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
#include<sstream>
#include<algorithm>

using namespace std;

class Table
{
private:

	typedef struct _conlumn {
		string conlumn_name;
		vector<string> data;
		int max_wide;
	}Conlumn;

	const string UNK{ "UNK" };
	ofstream fout;
	void Print_Line();
	void Get_Sort_temp(vector<int> & temp, vector<string> conlumn_order);
public:
	string File_name;
	string table_name;
	vector<Conlumn> conlumn;
	void Inset(vector<string> order_array);
	void Delete(vector<string> order_array);
	void Update(vector<string> order_array);
	void Select(vector<string> order_array);
	void Get_Filename(string name);
	void File_Write();
	void Print_table();
	void Divide_string(string str,vector<string>& str_array);
	bool Check_conlumn(vector<string> check_help);
	int Check_conlumn(string check_help);
	bool Check_value(string check_value, vector<string> check_container);
	Table(const Table& origin);
	Table();
	~Table();
};
