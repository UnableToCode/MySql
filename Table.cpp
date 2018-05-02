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
	bool flag{ true };
	int Inset_mod{};
	vector<string> conlumn_help;
	vector<string> vaule_help;
	stringstream ss;
	string input_help;
	string str_help;

	if (order_array.size() > 4) {
		if (order_array[3] == "VALUES")
			Inset_mod = 1;
		else if (order_array[4] == "VALUES")
			Inset_mod = 2;
	}

	switch (Inset_mod)
	{
	case 1:
		str_help = order_array[4];
		Divide_string(str_help, vaule_help);
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
		Divide_string(str_help, vaule_help);
		str_help = order_array[3];
		Divide_string(str_help, conlumn_help);
		flag = Check_conlumn(conlumn_help);
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
	string conlumn_if{};
	string value_if{};
	string if_mod{};
	bool flag{ false };
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
		
		conlumn_if = order_array[4];
		value_if = order_array[6];
		if_mod = order_array[5];
		if (if_mod == "=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a == b; });
		else if (if_mod == ">")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b; });
		else if (if_mod == "<")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b; });
		else if (if_mod == ">=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b || a == b; });
		else if (if_mod == "<=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b || a == b; });
		else if (if_mod == "!=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a != b; });
		else {
			cout << "Unknown order, check order DELETE!" << endl;
			break;
		}
		if (!flag)
			cout << "No such conlumn!" << endl;
		else if (index.size() == 0)
			cout << "Not found this value!" << endl;
		else {
			for (int j = index.size() - 1; j >= 0; j--) {
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
	vector<int> index;
	string conlumn_if{};
	string value_if{};
	string if_mod{};
	int Update_mod{};
	bool flag{ true };

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
		flag = Check_conlumn(conlumn_help);
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
		if_mod = order_array[order_array.size() - 2];
		if (if_mod == "=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a == b; });
		else if(if_mod==">")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b; });
		else if (if_mod == "<")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b; });
		else if (if_mod == ">=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b || a == b; });
		else if (if_mod == "<=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b || a == b; });
		else if (if_mod == "!=")
			flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a != b; });
		else {
			cout << "Unknown order, check order UPDATE!" << endl;
			break;
		}
		if (!flag)
			cout << "No such conlumn!" << endl;
		else if (index.size() == 0)
			cout << "Not found this value!" << endl;
		else {
			for (int i = 0; i < conlumn_help.size(); i++) {
				for (int j = 0; j < conlumn.size(); j++)
					if (conlumn[j].conlumn_name == conlumn_help[i]) {
						for (int k = 0; k < index.size(); k++)
							conlumn[j].data[index[k]] = value_help[i];
						break;
					}
			}
			Print_table();
		}
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
	vector<string> conlumn_order;
	vector<int> temp;
	vector<int> index;
	Conlumn conlumn_input;
	string conlumn_if{};
	string value_if{};
	string str_help{};
	string input_help{};
	string file_name{};
	string if_mod{};
	int select_mod{};
	int max_length{};
	bool flag{ true };

	if (order_array.size() > 3) {
		if (order_array.size() == 4)
			select_mod = 1;
		else if (order_array.size() == 5){
			if(order_array[1]=="DISTINCT")
				select_mod = 2;
		}
		else if (order_array.size() == 8) {
			if (order_array[4] == "ORDER"&&order_array[5] == "BY" && (order_array.back() == "ASC" || order_array.back() == "DESC"))
				select_mod = 3;
			else if (order_array[4] == "WHERE")
				select_mod = 4;
		}
		else if (order_array.size() == 6|| order_array.size() == 10) {
			if (order_array[order_array.size() - 2] == "TO")
				select_mod = 5;
		}

	}

	switch (select_mod)
	{
	case 1:
		if (order_array[1] == "*")
			Print_table();
		else {
			str_help = order_array[1];
			Divide_string(str_help, conlumn_help);
			flag = Check_conlumn(conlumn_help);
			if (!flag) {
				cout << "Erro:include Unknown conlumn!" << endl;
				break;
			}
			else {
				Get_table_help(table_help, conlumn_help);
				table_help.Print_table();
			}
		}
		break;
	case 2:
		str_help = order_array[2];
		if (str_help == "*")
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn_help.push_back(conlumn[i].conlumn_name);
			}
		else {
			Divide_string(str_help, conlumn_help);
			flag = Check_conlumn(conlumn_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown conlumn!" << endl;
			break;
		}
		else {
			Get_table_help(table_help, conlumn_help);
			for (int i = 0; i < table_help.conlumn.size(); i++) {
				sort(table_help.conlumn[i].data.begin(), table_help.conlumn[i].data.end(), [](string a, string b) {return (a.length() < b.length() || a.length() == b.length() && a < b); });
				table_help.conlumn[i].data.erase(unique(table_help.conlumn[i].data.begin(), table_help.conlumn[i].data.end()), table_help.conlumn[i].data.end());
			}
			max_length = table_help.conlumn[0].data.size();
			for (int i = 0; i < table_help.conlumn.size(); i++) {
				if (max_length < table_help.conlumn[i].data.size())
					max_length = table_help.conlumn[i].data.size();
			}
			for (int i = 0; i < table_help.conlumn.size(); i++) {
				while (table_help.conlumn[i].data.size() < max_length)
					table_help.conlumn[i].data.push_back(" ");
			}
			table_help.Print_table();
		}
			break;
	case 3:
		str_help = order_array[1];
		if(str_help=="*")
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn_help.push_back(conlumn[i].conlumn_name);
			}
		else {
			Divide_string(str_help, conlumn_help);
			flag = Check_conlumn(conlumn_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown conlumn!" << endl;
			break;
		}
		str_help = order_array[6];
		Divide_string(str_help, conlumn_order);
		flag = Check_conlumn(conlumn_order);
		if (!flag) {
			cout << "Erro:include Unknown conlumn!" << endl;
			break;
		}
		else {
			Get_Sort_temp(temp, conlumn_order);
			for (int i = 0; i < conlumn_help.size(); i++) {
				conlumn_input.conlumn_name = conlumn_help[i];
				for (int j = 0; j < conlumn.size(); j++) {
					if (conlumn_input.conlumn_name == conlumn[j].conlumn_name) {
						if(order_array.back()=="ASC")
							for (int k = 0; k < temp.size(); k++) {
								conlumn_input.data.push_back(conlumn[j].data[temp[k]]);
							}
						else if (order_array.back() == "DESC") 
							for (int k = temp.size() - 1; k >= 0; k--) {
								conlumn_input.data.push_back(conlumn[j].data[temp[k]]);
							}
						table_help.conlumn.push_back(conlumn_input);
						break;
					}
				}
				conlumn_input.data.clear();
			}
			table_help.Print_table();
		}
		break;
	case 4:
		str_help = order_array[1];
		conlumn_if = order_array[5];
		value_if = order_array.back();
		if (str_help == "*")
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn_help.push_back(conlumn[i].conlumn_name);
			}
		else
			Divide_string(str_help, conlumn_help);
		flag = Check_conlumn(conlumn_help);
		if(!flag)
			cout << "Erro:include Unknown conlumn!" << endl;
		else {
			if_mod = order_array[6];
			if (if_mod == "=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a == b; });
			else if (if_mod == ">")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b; });
			else if (if_mod == "<")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b; });
			else if (if_mod == ">=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b || a == b; });
			else if (if_mod == "<=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b || a == b; });
			else if (if_mod == "!=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a != b; });
			else {
				cout << "Unknown order, check order SELECT!" << endl;
				break;
			}
			if (!flag)
				cout << "No such conlumn!" << endl;
			else if (index.size() == 0)
				cout << "Not found this value!" << endl;
			else {
				Get_table_help(table_help, conlumn_help, index);
				table_help.Print_table();
			}
		}
		break;
	case 5:
		file_name = order_array.back();
		str_help = order_array[1];
		if (str_help == "*")
			for (int i = 0; i < conlumn.size(); i++) {
				conlumn_help.push_back(conlumn[i].conlumn_name);
			}
		else {
			Divide_string(str_help, conlumn_help);
			flag = Check_conlumn(conlumn_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown conlumn!" << endl;
			break;
		}
		if (order_array[4] == "WHERE") {
			conlumn_if = order_array[5];
			value_if = order_array[7];
			if_mod = order_array[6];
			if (if_mod == "=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a == b; });
			else if (if_mod == ">")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b; });
			else if (if_mod == "<")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b; });
			else if (if_mod == ">=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a > b || a == b; });
			else if (if_mod == "<=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a < b || a == b; });
			else if (if_mod == "!=")
				flag = Get_index(conlumn_if, value_if, index, [](string a, string b) {return a != b; });
			else {
				cout << "Unknown order, check order SELECT!" << endl;
				break;
			}
			if (!flag)
				cout << "No such conlumn!" << endl;
			else if (index.size() == 0)
				cout << "Not found this value!" << endl;
			else {
				fin.open(file_name);
				if (fin.is_open()) {
					cout << "Erro:file has been existed!" << endl;
					fin.close();
					break;
				}
				Get_table_help(table_help, conlumn_help, index);
				table_help.Print_table();
				table_help.Get_Filename(file_name);
				table_help.File_Write();
				cout << "File Write Success!" << endl;
			}
		}
		else {
			fin.open(file_name);
			if (fin.is_open()) {
				cout << "Erro:file has been existed!" << endl;
				fin.close();
				break;
			}
			Get_table_help(table_help, conlumn_help);
			table_help.Print_table();
			table_help.Get_Filename(file_name);
			table_help.File_Write();
			cout << "File Write Success!" << endl;
		}
		break;
	default:
		cout << "Erro:Unknown order, check order SELECT!" << endl;
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
		conlumn[i].max_wide = conlumn[i].conlumn_name.length();
		if (conlumn[i].max_wide < UNK.length())
			conlumn[i].max_wide = UNK.length();
		for (int j = 0; j < conlumn[i].data.size(); j++) {
			if (conlumn[i].max_wide < conlumn[i].data[j].length())
				conlumn[i].max_wide = conlumn[i].data[j].length();
		}
	}

	for (int i = 0; i < conlumn.size(); i++) {
		fout << left<< setw(conlumn[i].max_wide + 2) << conlumn[i].conlumn_name;
	}
	for (int i = 0; i < conlumn[0].data.size(); i++) {
		fout << endl;
		for (int j = 0; j < conlumn.size(); j++) {
			if (conlumn[j].data[i] != " ")
				fout << left << setw(conlumn[j].max_wide + 2) << conlumn[j].data[i];
			else
				fout << left << setw(conlumn[j].max_wide + 2) << UNK;
		}
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
	for (int i = 0; i < conlumn[0].data.size(); i++) {
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
void Table::Get_Sort_temp(vector<int>& temp, vector<string> conlumn_order)
{
	bool flag{ false };
	temp.push_back(0);
	vector<int> conlumn_order_index;
	for (int i = 0; i < conlumn_order.size(); i++) {
		for (int j = 0; j < conlumn.size(); j++) {
			if (conlumn_order[i] == conlumn[j].conlumn_name) {
				conlumn_order_index.push_back(j);
			}
		}
	}
	
	for (int i = 1; i < conlumn[conlumn_order_index[0]].data.size(); i++) {
		for (int j = 0; j < temp.size(); j++) {
			if (conlumn[conlumn_order_index[0]].data[i] < conlumn[conlumn_order_index[0]].data[temp[j]]) {
				temp.insert(temp.begin() + j, i);
				break;
			}
			else if (conlumn[conlumn_order_index[0]].data[i] == conlumn[conlumn_order_index[0]].data[temp[j]] && conlumn_order_index.size() > 1) {
				for (int k = 1; k < conlumn_order_index.size(); k++) {
					if (conlumn[conlumn_order_index[k]].data[i] == conlumn[conlumn_order_index[k]].data[temp[j]])
						continue;
					else if (conlumn[conlumn_order_index[k]].data[i] > conlumn[conlumn_order_index[k]].data[temp[j]])
						break;
					else {
						temp.insert(temp.begin() + j, i);
						flag = true;
						break;
					}
				}
			}
			if (flag) {
				flag = false;
				break;
			}
			if (j == temp.size() - 1) {
				temp.push_back(i);
				break;
			}
		}
	}
}

void Table::Divide_string(string str, vector<string>& str_array)
{
	stringstream ss;
	string input_help{};
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '(' || str[i] == ')' || str[i] == ',')
			str[i] = ' ';
	}
	ss.str(str);
	while (ss >> input_help) {
		str_array.push_back(input_help);
	}
	ss.clear();
}

bool Table::Check_conlumn(vector<string> check_help)
{
	bool flag{ false };
	for (int i = 0; i < check_help.size(); i++) {
		flag = false;
		for (int j = 0; j < conlumn.size(); j++)
			if (conlumn[j].conlumn_name == check_help[i]) {
				flag = true;
				break;
			}
		if (!flag)
			break;
	}
	return flag;
}

bool Table::Get_index(string check_help, string value_help, vector<int>& index, function<bool(string, string)> cmp)
{
	bool flag{ false };
	for (int i = 0; i < conlumn.size(); i++) {
		if (conlumn[i].conlumn_name == check_help) {
			for (int j = 0; j<conlumn[i].data.size(); j++)
				if (cmp(conlumn[i].data[j] , value_help))
					index.push_back(j);			
		flag = true;
		break;
		}
	}
	return flag;
}

void Table::Get_table_help(Table & table_help, vector<string> conlumn_help, vector<int> index)
{
	Conlumn conlumn_input;
	for (int i = 0; i < conlumn_help.size(); i++) {
		conlumn_input.conlumn_name = conlumn_help[i];
		table_help.conlumn.push_back(conlumn_input);
	}
	for (int i = 0; i < table_help.conlumn.size(); i++) {
		for (int j = 0; j < conlumn.size(); j++) {
			if (table_help.conlumn[i].conlumn_name == conlumn[j].conlumn_name) {
				for (int k = 0; k < index.size(); k++) {
					table_help.conlumn[i].data.push_back(conlumn[j].data[index[k]]);
				}
			}
		}
	}
}

void Table::Get_table_help(Table & table_help, vector<string> conlumn_help)
{
	for (int i = 0; i < conlumn_help.size(); i++) {
		for (int j = 0; j < conlumn.size(); j++)
			if (conlumn[j].conlumn_name == conlumn_help[i]) {
				table_help.conlumn.push_back(conlumn[j]);
			}
	}
}