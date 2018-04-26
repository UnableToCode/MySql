#include "Table.h"

Table::Table(const Table& origin)
{
	File_name = origin.File_name;
	table_name = origin.table_name;
	conlumn = origin.conlumn;
}

Table::Table()
{
}
Table::~Table()
{
}

void Table::Inset(vector<string> order_array)
{
	bool flag{ false };
	int Inset_mod{};
	vector<string> conlumn_help;
	vector<string> vaule_help;
	stringstream ss;
	string input_help;
	string str_help;
	if (order_array[3] == "VALUES")
		Inset_mod = 1;
	if (order_array[4] == "VALUES")
		Inset_mod = 2;
	switch (Inset_mod)
	{
	case 1:
		str_help = order_array[4];
		for (int i = 0; i < str_help.size(); i++) {
			if (str_help[i] == '(' || str_help[i] == ')' || str_help[i] == ',')
				str_help[i] = ' ';
		}
		ss.str(str_help);
		while (ss >> input_help) {
			vaule_help.push_back(input_help);
		}
		if (vaule_help.size() != conlumn.size()) {
			fout << "Erro:the number of vaule is not as same as it of conlumn!" << endl;
		}
		else {
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn[i].data.push_back(vaule_help[i]);
			}

		}

		break;
	case 2:
		str_help = order_array[5];
		for (int i = 0; i < str_help.size(); i++) {
			if (str_help[i] == '(' || str_help[i] == ')' || str_help[i] == ',')
				str_help[i] = ' ';
		}
		ss.str(str_help);
		while (ss >> input_help) {
			vaule_help.push_back(input_help);
		}
		ss.clear();
		str_help = order_array[3];
		for (int i = 0; i < str_help.size(); i++) {
			if (str_help[i] == '(' || str_help[i] == ')' || str_help[i] == ',')
				str_help[i] = ' ';
		}
		ss.str(str_help);
		while (ss >> input_help) {
			conlumn_help.push_back(input_help);
		}
		ss.clear();
		for (int i = 0; i < conlumn_help.size(); i++) {
			flag = false;
			for (int j = 0; j<conlumn.size(); j++)
				if (conlumn[j].conlumn_name == conlumn_help[i]) {
					flag = true;
					break;
				}
			if (!flag)
				break;
		}
		if (vaule_help.size() != conlumn_help.size()) {
			fout << "Erro:the number of vaule is not as same as it of conlumn!" << endl;
		}
		else if (flag) {
			for (int i = 0; i < conlumn.size(); i++) {
				flag = false;
				for (int j = 0; j < conlumn_help.size(); j++)
					if (conlumn[i].conlumn_name == conlumn_help[j]) {
						conlumn[i].data.push_back(vaule_help[j]);
						if (conlumn[i].max_wide < vaule_help[j].length())
							conlumn[i].max_wide = vaule_help[j].length();
						flag = true;	
						break;
					}
					if(!flag)
						conlumn[i].data.push_back(" ");
			}
		}
		else
			fout << "Erro:include Unknown conlumn!" << endl;
		
		break;
	default:
		fout << "Erro:Unknown order, check order INSERT INTO!" << endl;
		break;
	}
}

void Table::Delete(vector<string> order_array)
{

}

void Table::Update(vector<string> order_array)
{
}

void Table::Select(vector<string> order_array)
{
}

void Table::Get_Filename(string name)
{
	File_name = name;
}

void Table::File_Write()
{
	fout.open(File_name);
	if (!fout.is_open()) {
		cerr << "open file false!" << endl;
		exit(0);
	}

	for (int i = 0; i < conlumn.size(); i++) {
		fout << left<< setw(conlumn[i].max_wide + 2) << conlumn[i].conlumn_name;
	}
	fout << endl;
	for (int i = 0; i < conlumn[1].data.size(); i++) {
		for (int j = 0; j < conlumn.size(); j++) {
			if (conlumn[j].data[i] != " ")
				fout << left << setw(conlumn[j].max_wide + 2) << conlumn[j].data[i];
			else
				fout << left << setw(conlumn[j].max_wide + 2) << "UNK";
		}
		if(i!= conlumn[1].data.size()-1)
			fout << endl;
	}
	fout.close();
}

void Table::Print_Line()
{
	cout << "+-----+";
	for (int i = 0; i < conlumn.size(); i++) {
		for (int j = 0; j < conlumn[i].max_wide + 3; j++) {
			cout << "-";
		}
		cout << "+";
	}
	cout << endl;
}

void Table::Print_table()
{
	Print_Line();
	cout << "| " << left << setw(4) << "ID"<<"|";
	for (int i = 0; i < conlumn.size(); i++) {
		cout << " " << left << setw(conlumn[i].max_wide + 2) << conlumn[i].conlumn_name<<"|";
	}
	cout << endl;
	Print_Line();
	for (int i = 0; i < conlumn[1].data.size(); i++) {
		cout << "| " << left << setw(4) << i + 1 << "|";
		for (int j = 0; j < conlumn.size(); j++) {
			if (conlumn[j].data[i] != " ")
				cout << " " << left << setw(conlumn[j].max_wide + 2) << conlumn[j].data[i] << "|";
		}
		cout << endl;
	}
	if (conlumn.size() != 0)
		if (conlumn[0].data.size() != 0)
			Print_Line();
}