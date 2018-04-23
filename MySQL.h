#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<list>

using namespace std;

class Table;
class MySQL
{
private:
	list<Table> table;//存放TABLE
	list<Table>::iterator it;//table的迭代器
	vector<string> read_file;//存放已读文件的文件名
	ofstream fout;
	ifstream fin;
	const string tip{ "(mysql)==>" };
public:
	void start();
	void Create();//创建一个TABLE
	void List();//查看当前TABLE的数量以及每个TABLE的行列和列标题
	void Drop(string order);//删除一个TABLE
	MySQL();
	~MySQL();
};
