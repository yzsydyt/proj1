#include"std_lib_facilities.h"
int main() {
	ifstream f;
	f.open("1.txt");
	while (!f.eof()) {
		char s[100];
		f.getline(s, sizeof(s));
		cout << s << endl;
	}
	f.close();
	return 0;
}