#include "MySQL.h"



MySQL::MySQL()
{
}


MySQL::~MySQL()
{
}

void MySQL::start()
{
	order = {};
	getchar();
	while (order != "quit") {
		order = {};
		mod = 0;
		string input_help{};
		int i = 0;
		bool flag = false;

		cout << tip;
		getline(cin, order);
		stringstream ss(order);
		while (ss >> input_help)
			order_array.push_back(input_help);

		if (order_array.size() != 0) {
			if (order_array[0] == order1)
				mod = 1;
			if (order_array[0] == order2)
				mod = 2;
			if (order == order3)
				mod = 3;
			if (order_array[0] == order4)
				mod = 4;
			if (order_array[0] == order5)
				mod = 5;
			if (order_array[0] == order6)
				mod = 6;
			if (order_array[0] == order7)
				mod = 7;
		}

		switch (mod)
		{
		case 1:
			Create();
			break;
		case 2:
			if (table.size() == 0)
				PrintNoTable();
			else {
				Drop();
			}
			break;
		case 3:
			List();
			break;
		case 4:
			if (table.size() == 0)
				PrintNoTable();
			else {
				now_Tabel_name = order_array[2];
				for (it = table.begin(); it != table.end(); it++) {
					if (it->table_name == now_Tabel_name) {
						it->Inset(order_array);
						flag = true;
					}
				}
				if (!flag)
					PrintFail();
			}
			break;
		case 5:
			if (table.size() == 0)
				PrintNoTable();
			else {
				i = 0;
				while (order_array[i] != "FROM"&&i < order_array.size())
					i++;
				if (i == order_array.size())
					cout << "Unknown order, check order DELETE!" << endl;
				else {
					now_Tabel_name = order_array[i + 1];
					for (it = table.begin(); it != table.end(); it++) {
						if (it->table_name == now_Tabel_name) {
							it->Delete(order_array);
							flag = true;
						}
					}
					if (!flag)
						PrintFail();
				}
			}
			break;
		case 6:
			if (table.size() == 0)
				PrintNoTable();
			else {		
				now_Tabel_name = order_array[1];
				for (it = table.begin(); it != table.end(); it++) {
					if (it->table_name == now_Tabel_name) {
						it->Update(order_array);
						flag = true;
					}
				}
			}
			if (!flag)
				PrintFail();
		break;
		case 7:
			if (table.size() == 0)
				PrintNoTable();
			else {
				i = 0;
				while (order_array[i] != "FROM"&&i < order_array.size())
					i++;
				if (i == order_array.size())
					cout << "Unknown order, check order SELECT!" << endl;
				else {
					now_Tabel_name = order_array[i + 1];
					for (it = table.begin(); it != table.end(); it++) {
						if (it->table_name == now_Tabel_name) {
							it->Select(order_array);
							flag = true;
						}
					}
					if (!flag)
						PrintFail();
				}
			}
			break;
		default:
			if (order != "quit")
				cout << "Erro: Unknown order!" << endl;
			break;
		}
		order_array.clear();
	}

	for (it = table.begin(); it != table.end(); it++) {
		it->File_Write();
	}
	table.clear();
	read_file.clear();
}

void MySQL::Create()
{
	bool flag = true;
	int create_mod{};
	int size = order_array.size();
	vector<string> conlumn_help;
	string str_help{};
	Table table_help;
	int input{};

	if (order_array[size - 2] == "TO"&&order_array.size()==6)
		create_mod = 1;
	if (order_array[size - 2] == "FROM"&&order_array.size() == 5)
		create_mod = 2;
	File_name = order_array[size - 1];
	table_help.table_name = order_array[2];
	table_help.Get_Filename(File_name);
	for (it = table.begin(); it != table.end(); it++) {
		if (it->table_name == table_help.table_name) {
			cout << "Erro:table name exist!" << endl;
			create_mod = 3;
		}
	}

	switch (create_mod)
	{
	case 1:
		fin.open(File_name);
		if (fin.is_open()) {
			cout << "File already exists, whether replace it, in put 1 to agree, other to give up" << endl;
			cin >> input;
			getchar();
			if (input != 1)
				flag = false;
			fin.close();
		}
		if (flag) {		
			str_help = order_array[3];
			Divide_string(str_help, conlumn_help);
			table_help.conlumn.resize(conlumn_help.size());
			for (int i = 0; i < conlumn_help.size(); i++) 
				table_help.conlumn[i].conlumn_name = conlumn_help[i];
			table_help.File_Write();
			table.push_back(table_help);
			table_help.Print_table();
			read_file.push_back(File_name);
		}
		
		break;
	case 2:
		for (int i = 0; i < read_file.size(); i++) {
			if (read_file[i] == File_name) {
				cout << "Erro:File has been read by other tables!" << endl;
				flag = false;
				break;
			}
		}
		if (flag) {
			fin.open(File_name);
			if (!fin.is_open()) {
				cout << "open file false,check whether file exists" << endl;
			}
			else {			
				getline(fin, str_help);
				if (str_help.empty()) {
					cout << "Erro:file is empty!" << endl;
					break;
				}
				Divide_string(str_help, conlumn_help);
				table_help.conlumn.resize(conlumn_help.size());
				for (int i = 0; i < conlumn_help.size(); i++)
					table_help.conlumn[i].conlumn_name = conlumn_help[i];
				while(!fin.eof()){
					conlumn_help.clear();
					getline(fin, str_help);
					Divide_string(str_help, conlumn_help);
					for (int j = 0; j < table_help.conlumn.size(); j++) {
						if (conlumn_help[j] != UNK)
							table_help.conlumn[j].data.push_back(conlumn_help[j]);
						else
							table_help.conlumn[j].data.push_back(" ");
					}		
				}
				int data_length = table_help.conlumn[0].data.size();
				for (int i = 0; i < table_help.conlumn.size(); i++) {
					if (table_help.conlumn[i].data.size() != data_length) {
						cout << "Erro: format of file wrong" << endl;
						flag = false;
					}
				}
				if (flag) {
				table.push_back(table_help);
				read_file.push_back(File_name);
				table_help.Print_table();
				}
				fin.close();
			}
		}
		break;
	case 3:
		break;
	default:
		cout << "Unknown order,check order CREATE" << endl;
		break;
	}
}

void MySQL::List()
{
	cout << "total:" << table.size() << endl;
	for (it = table.begin(); it != table.end(); it++) {
		cout << "\t" << it->table_name << ": (" << it->conlumn.size() << "," << it->conlumn[0].data.size() << ") [";
		for (int i = 0; i < it->conlumn.size(); i++) {
			cout << it->conlumn[i].conlumn_name;
			if (i != it->conlumn.size() - 1)
				cout << ",";
		}
		cout << "]" << endl;
	}
}

void MySQL::Drop()
{
	if (order_array.size() != 3) {
		cout << "Erro:Unknown order,check order DROP" << endl;
	}
	else {
		bool flag{ false };
		string delet_table_name = order_array[2];
		for (it = table.begin(); it != table.end(); ) {
			if (it->table_name == delet_table_name) {
				for (int i = 0; i < read_file.size(); i++) {
					if (read_file[i] == it->File_name) {
						read_file.erase(read_file.begin() + i);
						break;
					}
				}
				it->File_Write();
				it = table.erase(it);
				flag = true;
			}
			else
				it++;
		}
		if (!flag)
			cout << "Erro:No such tables!" << endl;
	}
}

void MySQL::PrintFail()
{
	cout << "Erro: Check whether Table name exists!" << endl;
}

void MySQL::PrintNoTable()
{
	cout << "Erro: No Tables!" << endl;
}

void MySQL::Divide_string(string str, vector<string>& str_array)
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