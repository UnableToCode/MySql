#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class Table
{
private:

	typedef struct _conlumn {
		string conlumn_name;//列名
		vector<string> data;//该列中的数据
		int max_wide;//该列中最长数据长度，用于输出时控制输出长度
	}Conlumn;

	string table_name;//当前TABLE的名称
	vector<Conlumn> conlumn;//Table中的列
	ofstream fout;
	ifstream fin;
	string File_name;//Table对应的文件名

public:
	Table();
	~Table();
	void Inset(string order);//在TABLE中插入一行数据
	void Delete(string order);//删除TABLE中的一行数据
	void Update(string order);//更新TABLE中的数据
	void Select(string order);//选择展示TABLE中的数据
	void Get_Filename(string name);//获取TABLE的文件名
	void File_Write();//文件写入
};
