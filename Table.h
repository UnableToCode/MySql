#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class Table
{
private:
	typedef struct _table{
		vector<string> conlumn;
		vector<int> conlunm_num;
	}Table;
	vector<int> table_num;
	ofstream fout;
	ifstream fin;
	string File_name;
public:
	Table();
	~Table()'
	void Create();
	void Drop();
	void Inset();
	void Delete();
	void Update();
	void Select();
	void Get_Filename(string name);
}

