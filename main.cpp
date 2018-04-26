#include<iostream>
#include"MySQL.h"

using namespace std;

int main(int argc,char** argv){
	bool is_start{ true };
	string start{};
	MySQL mysql;
	while (is_start) {
		cout << "~$";
		cin >> start;
		if (start == "mySQL") {
			mysql.start();
		}
		if (start == "quit")
			is_start = false;
	}

	return 0;
}
