#include<iostream>
#include"Table.h"
#include"MainWindows.h"

using namespace std;

int main(int argc,char** argv){
	bool is_start{ false };
	string start{};
	MySQL mysql;
	while (true) {
		cout << "~$";
		cin >> start;
		if (start == "mySQL") {
			mysql.start();
		}
	}

	return 0;
}
