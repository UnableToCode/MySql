#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<list>
#include<sstream>
#include"Table.h"

using namespace std;

class MySQL
{
private:
	const string tip{ "(mysql)==>" };
	const string order1{ "CREATE" };
	const string order2{ "DROP" };
	const string order3{ "TABLE LIST" };
	const string order4{ "INSERT" };
	const string order5{ "DELETE" };
	const string order6{ "UPDATE" };
	const string order7{ "SELECT" };
	const string UNK{ "UNK" };
	list<Table> table;
	list<Table> ::iterator it;
	vector<string> read_file;
	ofstream fout;
	ifstream fin;
	string order;
	string File_name;
	vector<string> order_array;
	int mod;
	string now_Tabel_name;
	void Create();
	void List();
	void Drop();
	void PrintFail();
	void PrintNoTable();
	void Divide_string(string str, vector<string>& str_array);
public:
	void start();
	MySQL();
	~MySQL();
};
