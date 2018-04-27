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

	if (order_array.size() > 3) {
		if (order_array[3] == "VALUES")
			Inset_mod = 1;
		else if (order_array[4] == "VALUES")
			Inset_mod = 2;
	}

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
			cout << "Erro:the number of vaule is not as same as it of conlumn!" << endl;
		}
		else {
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn[i].data.push_back(vaule_help[i]);
			}
			Print_table();
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
			cout << "Erro:the number of vaule is not as same as it of conlumn!" << endl;
		}
		else if (flag) {
			for (int i = 0; i < conlumn.size(); i++) {
				flag = false;
				for (int j = 0; j < conlumn_help.size(); j++)
					if (conlumn[i].conlumn_name == conlumn_help[j]) {
						conlumn[i].data.push_back(vaule_help[j]);
						flag = true;	
						break;
					}
					if(!flag)
						conlumn[i].data.push_back(" ");
			}
			Print_table();
		}
		else
			cout << "Erro:include Unknown conlumn!" << endl;
		
		break;
	default:
		cout << "Erro:Unknown order, check order INSERT INTO!" << endl;
		break;
	}
}

void Table::Delete(vector<string> order_array)
{
	string conlumn_help{};
	string value_help{};
	bool flag_conlumn{ false };
	bool flag_value{ false };
	int delete_mod{};
	vector<int> index;

	if (order_array.size() > 3) {
		if (order_array[1] == "FROM")
			delete_mod = 1;
		else if (order_array[1] == "*")
			delete_mod = 2;
	}

	switch (delete_mod)
	{
	case 1:
		if (order_array[3] != "WHERE") {
			cout << "Unknown order, check order DELETE!" << endl;
			break;
		}
		
		conlumn_help = order_array[4];
		value_help = order_array[6];
		for (int i = 0; i < conlumn.size(); i++) {
			if (conlumn[i].conlumn_name == conlumn_help) {
				for (int j = 0; j<conlumn[i].data.size(); j++)
					if (conlumn[i].data[j]==value_help) {
						index.push_back(j);
						flag_value = true;
					}
				flag_conlumn = true;
				break;
			}
		}
		if (!flag_conlumn)
			cout << "No such conlumn!" << endl;
		else if (!flag_value)
			cout << "Not found this value!" << endl;
		else {
			for (int j = 0; j < index.size(); j++) {
				for (int i = 0; i < conlumn.size(); i++) {
					conlumn[i].data.erase(conlumn[i].data.begin() + index[j]);
				}
			}
			Print_table();
		}
		break;
	case 2:
		for (int i = 0; i < conlumn.size(); i++) {
			conlumn[i].data.clear();
		}
		Print_table();
		break;
	default:
		cout << "Erro:Unknown order, check order DELETE!" << endl;
		break;
		
	}
}

void Table::Update(vector<string> order_array)
{
	vector<string> conlumn_help;
	vector<string> value_help;
	vector<bool> is_change;
	string conlumn_if{};
	string value_if{};
	int Update_mod{};
	bool flag{ false };

	if (order_array.size() > 5 && order_array[2] == "SET") {
		if (order_array[order_array.size() - 4] == "WHERE")
			Update_mod = 2;
		else
			Update_mod = 1;
		for (int i = 0; i < order_array.size(); i++) {
			if (order_array[i].back() == ',')
				order_array[i].erase(order_array[i].end() - 1, order_array[i].end());
		}
		int i = 3;
		bool is_value{ false };
		while (i < order_array.size()) {
			if (order_array[i] == "WHERE")
				break;
			if (order_array[i] != "=") {
				if (!is_value) {
					conlumn_help.push_back(order_array[i]);
					is_value = true;
				}
				else {
					value_help.push_back(order_array[i]);
					is_value = false;
				}
			}
			i++;
		}
		for (int i = 0; i < conlumn_help.size(); i++) {
			flag = false;
			for (int j = 0; j < conlumn.size(); j++)
				if (conlumn[j].conlumn_name == conlumn_help[i]) {
					flag = true;
					break;
				}
			if (!flag)
				break;
		}
		if (!flag) {
			cout << "Erro:include Unknown conlumn!" << endl;
			Update_mod = 3;
		}

	}

	switch (Update_mod)
	{
	case 1:
		for (int i = 0; i < conlumn_help.size(); i++) {
			for (int j = 0; j < conlumn.size(); j++)
				if (conlumn[j].conlumn_name == conlumn_help[i]) {
					for (int k = 0; k < conlumn[j].data.size(); k++)
						conlumn[j].data[k] = value_help[i];
					break;
				}
		}
		Print_table();
		break;
	case 2:
		conlumn_if = order_array[order_array.size() - 3];
		value_if = order_array[order_array.size() - 1];
		for (int i = 0; i < conlumn.size(); i++) {
			if (conlumn[i].conlumn_name == conlumn_if) {
				for (int j = 0; j < conlumn[i].data.size(); j++) {
					if (conlumn[i].data[j] == value_if)
						is_change.push_back(true);
					else
						is_change.push_back(false);
				}
			}
		}
		for (int i = 0; i < conlumn_help.size(); i++) {
			for (int j = 0; j < conlumn.size(); j++)
				if (conlumn[j].conlumn_name == conlumn_help[i]) {
					for (int k = 0; k < conlumn[j].data.size(); k++)
						if(is_change[k])
							conlumn[j].data[k] = value_help[i];
					break;
				}
		}
		Print_table();
		break;
	case 3:
		break;
	default:
		cout << "Erro:Unknown order, check order UPDATE!" << endl;
		break;
	}
}

void Table::Select(vector<string> order_array)
{
	Table table_help;
	vector<string> conlumn_help;
	vector<string> value_help;
	Conlumn sort_help;
	string conlumn_if{};
	stringstream ss;
	int select_mod{};

	if (order_array.size() > 3) {
		if (order_array.size() == 4)
			select_mod = 1;
		else if (order_array.size() == 5)
			select_mod = 2;
		else if (order_array.size() == 8) {
			if (order_array[4] == "ORDER")
				select_mod = 3;
			else if (order_array[4] == "WHERE")
				select_mod = 4;
		}
		else if (order_array.size() == 6)
			if (order_array[4] == "TO")
				select_mod = 5;

	}

	switch (select_mod)
	{
	case 1:
		if (order_array[1] == "*")
			Print_table();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}

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
				fout << left << setw(conlumn[j].max_wide + 2) << UNK;
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
	for (int i = 0; i < conlumn.size(); i++) {
		conlumn[i].max_wide = conlumn[i].conlumn_name.length();
		if (conlumn[i].max_wide < UNK.length())
			conlumn[i].max_wide = UNK.length();
		for (int j = 0; j < conlumn[i].data.size(); j++) {
			if (conlumn[i].max_wide < conlumn[i].data[j].length())
				conlumn[i].max_wide = conlumn[i].data[j].length();
		}
	}
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
				cout << " " << left << setw(conlumn[j].max_wide + 2) << conlumn[j].data[i] << "|";
		}
		cout << endl;
	}
	if (conlumn.size() != 0)
		if (conlumn[0].data.size() != 0)
			Print_Line();
}