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
	list<Table> table;//���TABLE
	list<Table>::iterator it;//table�ĵ�����
	vector<string> read_file;//����Ѷ��ļ����ļ���
	ofstream fout;
	ifstream fin;
	const string tip{ "(mysql)==>" };
public:
	void start();
	void Create();//����һ��TABLE
	void List();//�鿴��ǰTABLE�������Լ�ÿ��TABLE�����к��б���
	void Drop(string order);//ɾ��һ��TABLE
	MySQL();
	~MySQL();
};
