#include "Table.h"

Table::Table(const Table& origin)
{
	File_name = origin.File_name;
	table_name = origin.table_name;
	column = origin.column;
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
	vector<string> column_help;
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
		if (vaule_help.size() != column.size()) {
			cout << "Erro:the number of vaule is not as same as it of column!" << endl;
		}
		else {
			for (int i = 0; i < column.size(); i++) {
				column[i].data.push_back(vaule_help[i]);
			}
			Print_table();
		}

		break;
	case 2:
		str_help = order_array[5];
		Divide_string(str_help, vaule_help);
		str_help = order_array[3];
		Divide_string(str_help, column_help);
		flag = Check_column(column_help);
		if (vaule_help.size() != column_help.size()) {
			cout << "Erro:the number of vaule is not as same as it of column!" << endl;
		}
		else if (flag) {
			for (int i = 0; i < column.size(); i++) {
				flag = false;
				for (int j = 0; j < column_help.size(); j++)
					if (column[i].column_name == column_help[j]) {
						column[i].data.push_back(vaule_help[j]);
						flag = true;	
						break;
					}
					if(!flag)
						column[i].data.push_back(" ");
			}
			Print_table();
		}
		else
			cout << "Erro:include Unknown column!" << endl;
		
		break;
	default:
		cout << "Erro:Unknown order, check order INSERT INTO!" << endl;
		break;
	}
}

void Table::Delete(vector<string> order_array)
{
	string column_if{};
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
		
		column_if = order_array[4];
		value_if = order_array[6];
		if_mod = order_array[5];
		if (if_mod == "=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a == b; });
		else if (if_mod == ">")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b; });
		else if (if_mod == "<")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b; });
		else if (if_mod == ">=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b || a == b; });
		else if (if_mod == "<=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b || a == b; });
		else if (if_mod == "!=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a != b; });
		else if(if_mod == "IN")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return b.find(a) != string::npos; });
		else {
			cout << "Unknown order, check order DELETE!" << endl;
			break;
		}
		if (!flag)
			cout << "No such column!" << endl;
		else if (index.size() == 0)
			cout << "Not found this value!" << endl;
		else {
			for (int j = index.size() - 1; j >= 0; j--) {
				for (int i = 0; i < column.size(); i++) {
					column[i].data.erase(column[i].data.begin() + index[j]);
				}
			}
			Print_table();
		}
		break;
	case 2:
		for (int i = 0; i < column.size(); i++) {
			column[i].data.clear();
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
	vector<string> column_help;
	vector<string> value_help;
	vector<int> index;
	string column_if{};
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
					column_help.push_back(order_array[i]);
					is_value = true;
				}
				else {
					value_help.push_back(order_array[i]);
					is_value = false;
				}
			}
			i++;
		}
		flag = Check_column(column_help);
		if (!flag) {
			cout << "Erro:include Unknown column!" << endl;
			Update_mod = 3;
		}

	}

	switch (Update_mod)
	{
	case 1:
		for (int i = 0; i < column_help.size(); i++) {
			for (int j = 0; j < column.size(); j++)
				if (column[j].column_name == column_help[i]) {
					for (int k = 0; k < column[j].data.size(); k++)
						column[j].data[k] = value_help[i];
					break;
				}
		}
		Print_table();
		break;
	case 2:
		column_if = order_array[order_array.size() - 3];
		value_if = order_array[order_array.size() - 1];
		if_mod = order_array[order_array.size() - 2];
		if (if_mod == "=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a == b; });
		else if(if_mod==">")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b; });
		else if (if_mod == "<")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b; });
		else if (if_mod == ">=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b || a == b; });
		else if (if_mod == "<=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b || a == b; });
		else if (if_mod == "!=")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return a != b; });
		else if (if_mod == "IN")
			flag = Get_index(column_if, value_if, index, [](string a, string b) {return b.find(a) != string::npos; });
		else {
			cout << "Unknown order, check order UPDATE!" << endl;
			break;
		}
		if (!flag)
			cout << "No such column!" << endl;
		else if (index.size() == 0)
			cout << "Not found this value!" << endl;
		else {
			for (int i = 0; i < column_help.size(); i++) {
				for (int j = 0; j < column.size(); j++)
					if (column[j].column_name == column_help[i]) {
						for (int k = 0; k < index.size(); k++)
							column[j].data[index[k]] = value_help[i];
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
	vector<string> column_help;
	vector<string> column_order;
	vector<int> temp;
	vector<int> index;
	Column column_input;
	string column_if{};
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
			Divide_string(str_help, column_help);
			flag = Check_column(column_help);
			if (!flag) {
				cout << "Erro:include Unknown column!" << endl;
				break;
			}
			else {
				Get_table_help(table_help, column_help);
				table_help.Print_table();
			}
		}
		break;
	case 2:
		str_help = order_array[2];
		if (str_help == "*")
			for (int i = 0; i < column.size(); i++) {
				column_help.push_back(column[i].column_name);
			}
		else {
			Divide_string(str_help, column_help);
			flag = Check_column(column_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown column!" << endl;
			break;
		}
		else {
			Get_table_help(table_help, column_help);
			for (int i = 0; i < table_help.column.size(); i++) {
				sort(table_help.column[i].data.begin(), table_help.column[i].data.end(), [](string a, string b) {return (a.length() < b.length() || a.length() == b.length() && a < b); });
				table_help.column[i].data.erase(unique(table_help.column[i].data.begin(), table_help.column[i].data.end()), table_help.column[i].data.end());
			}
			max_length = table_help.column[0].data.size();
			for (int i = 0; i < table_help.column.size(); i++) {
				if (max_length < table_help.column[i].data.size())
					max_length = table_help.column[i].data.size();
			}
			for (int i = 0; i < table_help.column.size(); i++) {
				while (table_help.column[i].data.size() < max_length)
					table_help.column[i].data.push_back(" ");
			}
			table_help.Print_table();
		}
			break;
	case 3:
		str_help = order_array[1];
		if(str_help=="*")
			for (int i = 0; i < column.size(); i++) {
				column_help.push_back(column[i].column_name);
			}
		else {
			Divide_string(str_help, column_help);
			flag = Check_column(column_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown column!" << endl;
			break;
		}
		str_help = order_array[6];
		Divide_string(str_help, column_order);
		flag = Check_column(column_order);
		if (!flag) {
			cout << "Erro:include Unknown column!" << endl;
			break;
		}
		else {
			Get_Sort_temp(temp, column_order);
			for (int i = 0; i < column_help.size(); i++) {
				column_input.column_name = column_help[i];
				for (int j = 0; j < column.size(); j++) {
					if (column_input.column_name == column[j].column_name) {
						if(order_array.back()=="ASC")
							for (int k = 0; k < temp.size(); k++) {
								column_input.data.push_back(column[j].data[temp[k]]);
							}
						else if (order_array.back() == "DESC") 
							for (int k = temp.size() - 1; k >= 0; k--) {
								column_input.data.push_back(column[j].data[temp[k]]);
							}
						table_help.column.push_back(column_input);
						break;
					}
				}
				column_input.data.clear();
			}
			table_help.Print_table();
		}
		break;
	case 4:
		str_help = order_array[1];
		column_if = order_array[5];
		value_if = order_array.back();
		if (str_help == "*")
			for (int i = 0; i < column.size(); i++) {
				column_help.push_back(column[i].column_name);
			}
		else
			Divide_string(str_help, column_help);
		flag = Check_column(column_help);
		if(!flag)
			cout << "Erro:include Unknown column!" << endl;
		else {
			if_mod = order_array[6];
			if (if_mod == "=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a == b; });
			else if (if_mod == ">")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b; });
			else if (if_mod == "<")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b; });
			else if (if_mod == ">=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b || a == b; });
			else if (if_mod == "<=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b || a == b; });
			else if (if_mod == "!=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a != b; });
			else if (if_mod == "IN")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return b.find(a) != string::npos; });
			else {
				cout << "Unknown order, check order SELECT!" << endl;
				break;
			}
			if (!flag)
				cout << "No such column!" << endl;
			else if (index.size() == 0)
				cout << "Not found this value!" << endl;
			else {
				Get_table_help(table_help, column_help, index);
				table_help.Print_table();
			}
		}
		break;
	case 5:
		file_name = order_array.back();
		str_help = order_array[1];
		if (str_help == "*")
			for (int i = 0; i < column.size(); i++) {
				column_help.push_back(column[i].column_name);
			}
		else {
			Divide_string(str_help, column_help);
			flag = Check_column(column_help);
		}
		if (!flag) {
			cout << "Erro:include Unknown column!" << endl;
			break;
		}
		if (order_array[4] == "WHERE") {
			column_if = order_array[5];
			value_if = order_array[7];
			if_mod = order_array[6];
			if (if_mod == "=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a == b; });
			else if (if_mod == ">")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b; });
			else if (if_mod == "<")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b; });
			else if (if_mod == ">=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a > b || a == b; });
			else if (if_mod == "<=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a < b || a == b; });
			else if (if_mod == "!=")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return a != b; });
			else if (if_mod == "IN")
				flag = Get_index(column_if, value_if, index, [](string a, string b) {return b.find(a) != string::npos; });
			else {
				cout << "Unknown order, check order SELECT!" << endl;
				break;
			}
			if (!flag)
				cout << "No such column!" << endl;
			else if (index.size() == 0)
				cout << "Not found this value!" << endl;
			else {
				fin.open(file_name);
				if (fin.is_open()) {
					cout << "Erro:file has been existed!" << endl;
					fin.close();
					break;
				}
				Get_table_help(table_help, column_help, index);
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
			Get_table_help(table_help, column_help);
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
	
	for (int i = 0; i < column.size(); i++) {
		column[i].max_wide = column[i].column_name.length();
		if (column[i].max_wide < UNK.length())
			column[i].max_wide = UNK.length();
		for (int j = 0; j < column[i].data.size(); j++) {
			if (column[i].max_wide < column[i].data[j].length())
				column[i].max_wide = column[i].data[j].length();
		}
	}

	for (int i = 0; i < column.size(); i++) {
		fout << left<< setw(column[i].max_wide + 2) << column[i].column_name;
	}
	for (int i = 0; i < column[0].data.size(); i++) {
		fout << endl;
		for (int j = 0; j < column.size(); j++) {
			if (column[j].data[i] != " ")
				fout << left << setw(column[j].max_wide + 2) << column[j].data[i];
			else
				fout << left << setw(column[j].max_wide + 2) << UNK;
		}
	}
	fout.close();
}

void Table::Print_Line()
{
	cout << "+-----+";
	for (int i = 0; i < column.size(); i++) {
		for (int j = 0; j < column[i].max_wide + 3; j++) {
			cout << "-";
		}
		cout << "+";
	}
	cout << endl;
}

void Table::Print_table()
{
	for (int i = 0; i < column.size(); i++) {
		column[i].max_wide = column[i].column_name.length();
		if (column[i].max_wide < UNK.length())
			column[i].max_wide = UNK.length();
		for (int j = 0; j < column[i].data.size(); j++) {
			if (column[i].max_wide < column[i].data[j].length())
				column[i].max_wide = column[i].data[j].length();
		}
	}
	Print_Line();
	cout << "| " << left << setw(4) << "ID"<<"|";
	for (int i = 0; i < column.size(); i++) {
		cout << " " << left << setw(column[i].max_wide + 2) << column[i].column_name<<"|";
	}
	cout << endl;
	Print_Line();
	for (int i = 0; i < column[0].data.size(); i++) {
		cout << "| " << left << setw(4) << i + 1 << "|";
		for (int j = 0; j < column.size(); j++) {
				cout << " " << left << setw(column[j].max_wide + 2) << column[j].data[i] << "|";
		}
		cout << endl;
	}
	if (column.size() != 0)
		if (column[0].data.size() != 0)
			Print_Line();
}
void Table::Get_Sort_temp(vector<int>& temp, vector<string> column_order)
{
	bool flag{ false };
	temp.push_back(0);
	vector<int> column_order_index;
	for (int i = 0; i < column_order.size(); i++) {
		for (int j = 0; j < column.size(); j++) {
			if (column_order[i] == column[j].column_name) {
				column_order_index.push_back(j);
			}
		}
	}
	
	for (int i = 1; i < column[column_order_index[0]].data.size(); i++) {
		for (int j = 0; j < temp.size(); j++) {
			if (column[column_order_index[0]].data[i] < column[column_order_index[0]].data[temp[j]]) {
				temp.insert(temp.begin() + j, i);
				break;
			}
			else if (column[column_order_index[0]].data[i] == column[column_order_index[0]].data[temp[j]] && column_order_index.size() > 1) {
				for (int k = 1; k < column_order_index.size(); k++) {
					if (column[column_order_index[k]].data[i] == column[column_order_index[k]].data[temp[j]])
						continue;
					else if (column[column_order_index[k]].data[i] > column[column_order_index[k]].data[temp[j]])
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

bool Table::Check_column(vector<string> check_help)
{
	bool flag{ false };
	for (int i = 0; i < check_help.size(); i++) {
		flag = false;
		for (int j = 0; j < column.size(); j++)
			if (column[j].column_name == check_help[i]) {
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
	for (int i = 0; i < column.size(); i++) {
		if (column[i].column_name == check_help) {
			for (int j = 0; j<column[i].data.size(); j++)
				if (cmp(column[i].data[j] , value_help))
					index.push_back(j);			
		flag = true;
		break;
		}
	}
	return flag;
}

void Table::Get_table_help(Table & table_help, vector<string> column_help, vector<int> index)
{
	Column column_input;
	for (int i = 0; i < column_help.size(); i++) {
		column_input.column_name = column_help[i];
		table_help.column.push_back(column_input);
	}
	for (int i = 0; i < table_help.column.size(); i++) {
		for (int j = 0; j < column.size(); j++) {
			if (table_help.column[i].column_name == column[j].column_name) {
				for (int k = 0; k < index.size(); k++) {
					table_help.column[i].data.push_back(column[j].data[index[k]]);
				}
			}
		}
	}
}

void Table::Get_table_help(Table & table_help, vector<string> column_help)
{
	for (int i = 0; i < column_help.size(); i++) {
		for (int j = 0; j < column.size(); j++)
			if (column[j].column_name == column_help[i]) {
				table_help.column.push_back(column[j]);
			}
	}
}