#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

class Table
{
private:

	typedef struct _conlumn {
		string conlumn_name;//����
		vector<string> data;//�����е�����
		int max_wide;//����������ݳ��ȣ��������ʱ�����������
	}Conlumn;

	string table_name;//��ǰTABLE������
	vector<Conlumn> conlumn;//Table�е���
	ofstream fout;
	ifstream fin;
	string File_name;//Table��Ӧ���ļ���

public:
	Table();
	~Table();
	void Inset(string order);//��TABLE�в���һ������
	void Delete(string order);//ɾ��TABLE�е�һ������
	void Update(string order);//����TABLE�е�����
	void Select(string order);//ѡ��չʾTABLE�е�����
	void Get_Filename(string name);//��ȡTABLE���ļ���
	void File_Write();//�ļ�д��
};
