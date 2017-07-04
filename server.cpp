#include <array>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include"std_lib_facilities.h"
using boost::asio::ip::tcp;
class token {
public:
	int val;
	double num;
	string s;//记录状态，数值等
};
token sale[1000000][15];
token buy[1000000][15];
int a[20];
token order[20];
int sale_num, buy_num;


void inputwork(char c[500]) {
	bool p1 = true;
	int j = 0;
	int x = 0;
	string s;
	for (int i = 0; i < 15; i++) {
		order[i].val = 0;
		order[i].num = 0;
		order[i].s = "";
	}
	while (c[j] != '|') {
		if (c[j] == '=') {
			stringstream f;
			f << s;
			f >> x;
			s == "";
			p1 = false;
		}
		else if (c[j] == ';') {
			int i = 0;
			while (a[i] != x&&i < 15) { i += 1; }
			if (a[i] == x) {
				if (i != 10 && i != 2 && i != 3) { order[i].s = s; }
				else if (i == 10 || i == 3) {
					stringstream ss;
					double x;
					ss << s;
					ss >> x;
					order[i].num = x;
				}
				else {
					stringstream ss;
					int x;
					ss << s;
					ss >> x;
					order[i].val = x;
				}
			}
			s = "";
			p1 = true;
		}
		else { s += c[j]; }
		j += 1;
	}
}


void initialize() {//初始化
	a[0] = 6;
	a[1] = 11;
	a[2] = 14;
	a[3] = 31;
	a[4] = 32;
	a[5] = 35;
	a[6] = 38;
	a[7] = 39;
	a[8] = 40;
	a[9] = 41;
	a[10] = 44;
	a[11] = 54;
	a[12] = 55;
	a[13] = 150;
	a[14] = 151;
	sale_num = 0;
	buy_num = 0;
	char c[500];
	ifstream f("saleorder.txt");
	while (!f.eof()) {
		f.getline(c, 500);
		inputwork(c);
		sale_num += 1;
	}
	f.close();
	ifstream f1("buyorder.txt");
	while (!f1.eof()) {
		f1.getline(c, 500);
		inputwork(c);
		buy_num += 1;
	}
	f1.close();
}


int main()
{
	initialize();
	boost::asio::io_service io_service;
	tcp::acceptor acc(io_service, tcp::endpoint(tcp::v6(), 1323));
	std::string message;

	while (1) {
		boost::system::error_code ignored;

		tcp::socket socket(io_service);
		acc.accept(socket);

		std::array<char, 500> input_buffer;
		std::size_t input_size = socket.read_some(
			boost::asio::buffer(input_buffer),
			ignored);

		if (input_buffer[0] == 'l'&&input_buffer[1] == 'o'&&input_buffer[2] == 'g'&&input_buffer[3] == 'i'&&input_buffer[4] == 'n') {//login
			bool p = false;
			std::string s = "";
			while (p == false) {
				std::array<char, 500> id;
				std::size_t id_size = socket.read_some(
					boost::asio::buffer(id),
					ignored);
				std::array<char, 500> password;
				std::size_t password_size = socket.read_some(
					boost::asio::buffer(password),
					ignored);
				ifstream f("admins.txt");
				while (!f.eof()) {
					char ids[500];
					char pass[500];
					char c[500];
					f.getline(ids, 500);
					f.getline(pass, 500);
					f.getline(c, 500);
					bool p1 = true;
					for (int i = 0; i < id_size; i++) {
						if (id[i] != ids[i]) { p1 = false; }
					}
					if (ids[id_size] != '|') { p1 = false; }
					for (int i = 0; i < password_size; i++) {
						if (password[i] != pass[i]) { p1 = false; }
					}
					if (pass[password_size] != '|') { p1 = false; }
					if (p1) { 
						p = true;
						int i = 0;
						while (c[i] != '|') {
							s += c[i];
							i += 1;
						}
					}
				}
				f.close();
				if (p == false) {
					boost::asio::write(socket, boost::asio::buffer("failed"), ignored);
				}
			}
			boost::asio::write(socket, boost::asio::buffer(s), ignored);
		}
		else if(input_buffer[0] == 'r'&&input_buffer[1] == 'e'&&input_buffer[2] == 'g'&&input_buffer[3] == 'i'&&input_buffer[4] == 's'){
			bool p = false;
			char c[500], c1[500];
			int num = 1;
			string idnumber;
			ifstream f("admins.txt");
			while (!f.eof()) {
				f.getline(c, 500);
				f.getline(c, 500);
				f.getline(c, 500);
				num += 1;
			}
			f.close();
			stringstream ss;
			ss << num;
			ss >> idnumber;
			string id_str="";
			while (p == false) {
				p = true;
				std::array<char, 500> id;
				std::size_t id_size = socket.read_some(
					boost::asio::buffer(id),
					ignored);
				ifstream f("admins.txt");
				while (!f.eof()) {
					f.getline(c, 500);
					f.getline(c1, 500);
					f.getline(c1, 500);
					bool p1 = true;
					for(int i=0;i<id_size;i++){
						if (id[i] != c[i]) { p1 = false; }
					}
					if (c[id_size] != '|') { p1 = false; }
					if (p1) { p = false; }
				}
				f.close();
				if (p == false) {
					boost::asio::write(socket, boost::asio::buffer("repeated"), ignored);
				}
				else {
					for (int i = 0; i < id_size; i++) { id_str += id[i]; }
				}
			}
			std::array<char, 500> password;
			std::size_t password_size = socket.read_some(
				boost::asio::buffer(password),
				ignored);
			ofstream ff("admins.txt",ios::app);
			id_str += "|\n";
			ff << "\n";
			ff << id_str;
			for (int i = 0; i < password_size; i++) { ff << password[i]; }
			ff << "|\n";
			ff << idnumber << "|";
			boost::asio::write(socket, boost::asio::buffer(idnumber), ignored);
		}
		else if (input_buffer[0] == 'o'&&input_buffer[1] == 'r'&&input_buffer[2] == 'd'&&input_buffer[3] == 'e'&&input_buffer[4] == 'r') {//order
			std::array<char, 500> orders;
			std::size_t orders_size = socket.read_some(
				boost::asio::buffer(orders),
				ignored);
			char c[500];
			for (int i = 0; i < orders_size; i++) {
				c[i] = orders[i];
			}
			inputwork(c);
			if (order[5].s == "D") {
				string s="";
				order[5].s = "8";
				order[7].s = "0";
				order[13].s = "0";
				for (int i = 0; i < 15; i++) {
					stringstream ss;
					ss << a[i];
					string s1;
					ss >> s1;
					s = s + s1 + "=";
					if (i == 10||i==3) {
						stringstream ss;
						ss << order[i].num;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else if(i==2){
						stringstream ss;
						ss << order[i].val;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else {
						s += order[i].s;
					}
					s += ";";
				}
				s += "|";
				boost::asio::write(socket, boost::asio::buffer(s), ignored);
				if (order[11].s == "1") {
					for (int i = 0; i < sale_num; i++)if(order[2].val>0){
						if (order[10].num > sale[i][10].num) {
							if (order[2].val > sale[i][2].val) {
								order[2].val -= sale[i][2].val;
								sale[i][2].val = 0;
								sale[i][3].num = order[10].num;
							}
							else {
								sale[i][2].val -= order[2].val;
								order[2].val = 0;
								sale[i][3].num = order[10].num;
							}
						}
					}
					int j = 0;
					while (sale[j][2].val == 0&&j<sale_num) { j += 1; }
					for (int i = j; i < sale_num; i++) {
						for (int k = 0; k < 15; k++) {
							sale[i - j][k].val = sale[i][k].val;
							sale[i - j][k].num = sale[i][k].num;
							sale[i - j][k].s = sale[i][k].s;
						}
					}
					sale_num -= j;
					if (order[2].val == 0) {
						order[13].s = "2";
						order[7].s = "2";
					}
					else {
						order[13].s = "1";
						order[7].s = "1";
						int j = 0;
						while (order[10].num >= buy[j][10].num&&j<buy_num) { j += 1; }
						for (int i = buy_num;i>j; i--) {
							for (int k = 0; k < 15; k++) {
								buy[i][k].num = buy[i - 1][k].num;
								buy[i][k].val = buy[i - 1][k].val;
								buy[i][k].s = buy[i - 1][k].s;
							}
						}
						buy_num += 1;
						for (int k = 0; k < 15; k++) {
							buy[j][k].num = order[k].num;
							buy[j][k].val = order[k].val;
							buy[j][k].s = order[k].s;
						}
					}
					s = "";
					for (int i = 0; i < 15; i++) {
						stringstream ss;
						ss << a[i];
						string s1;
						ss >> s1;
						s = s + s1 + "=";
						if (i == 10 || i == 3) {
							stringstream ss;
							ss << order[i].num;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else if (i == 2) {
							stringstream ss;
							ss << order[i].val;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else {
							s += order[i].s;
						}
						s += ";";
					}
					s += "|";
					boost::asio::write(socket, boost::asio::buffer(s), ignored);
				}
				else {
					for (int i = buy_num - 1; i >= 0; i--) if(order[2].val>0){
						if (buy[i][10].num > order[10].num) {
							if (order[2].val > buy[i][2].val) {
								order[2].val -= buy[i][2].val;
								buy[i][2].val = 0;
								buy[i][3].num = order[10].num;
							}
							else {
								buy[i][2].val -= order[2].val;
								buy[i][3].num = order[10].num;
								order[2].val = 0;
							}
						}
					}
					while (buy_num > 0 && buy[buy_num - 1][2].val == 0) { buy_num -= 1; }
					if (order[2].val == 0) {
						order[13].s = "2";
						order[7].s = "2";
					}
					else {
						order[13].s = "1";
						order[7].s = "1";
						int j = 0;
						while (order[10].num >= sale[j][10].num&&j<sale_num) { j += 1; }
						for (int i = sale_num; i>j; i--) {
							for (int k = 0; k < 15; k++) {
								sale[i][k].num = sale[i - 1][k].num;
								sale[i][k].val = sale[i - 1][k].val;
								sale[i][k].s = sale[i - 1][k].s;
							}
						}
						sale_num += 1;
						for (int k = 0; k < 15; k++) {
							sale[j][k].num = order[k].num;
							sale[j][k].val = order[k].val;
							sale[j][k].s = order[k].s;
						}
					}
					s = "";
					for (int i = 0; i < 15; i++) {
						stringstream ss;
						ss << a[i];
						string s1;
						ss >> s1;
						s = s + s1 + "=";
						if (i == 10 || i == 3) {
							stringstream ss;
							ss << order[i].num;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else if (i == 2) {
							stringstream ss;
							ss << order[i].val;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else {
							s += order[i].s;
						}
						s += ";";
					}
					s += "|";
					boost::asio::write(socket, boost::asio::buffer(s), ignored);
				}
				ofstream f("saleorder");
				for (int k = 0; k < sale_num; k++) {
					string s = "";
					for (int i = 0; i < 15; i++) {
						stringstream ss;
						ss << a[i];
						string s1;
						ss >> s1;
						s = s + s1 + "=";
						if (i == 10 || i == 3) {
							stringstream ss;
							ss << sale[k][i].num;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else if (i == 2) {
							stringstream ss;
							ss << sale[k][i].val;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else {
							s += sale[k][i].s;
						}
						s += ";";
					}
					s += "|";
					f << s;
				}
				f.close();

				ofstream f1("buyorder");
				for (int k = 0; k < buy_num; k++) {
					string s = "";
					for (int i = 0; i < 15; i++) {
						stringstream ss;
						ss << a[i];
						string s1;
						ss >> s1;
						s = s + s1 + "=";
						if (i == 10 || i == 3) {
							stringstream ss;
							ss << buy[k][i].num;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else if (i == 2) {
							stringstream ss;
							ss << buy[k][i].val;
							string s1;
							ss >> s1;
							s = s + s1;
						}
						else {
							s += buy[k][i].s;
						}
						s += ";";
					}
					s += "|";
					f1 << s;
				}
				f1.close();
			}
		}
		else if (input_buffer[0] == 'q'&&input_buffer[1] == 'u'&&input_buffer[2] == 'e'&&input_buffer[3] == 's'&&input_buffer[4] == 't') {
			stringstream ss;
			string s;
			ss << sale_num;
			ss >> s;
			boost::asio::write(socket, boost::asio::buffer(s), ignored);
			for (int k = 0; k < sale_num; k++) {
				string s = "";
				for (int i = 0; i < 15; i++) {
					stringstream ss;
					ss << a[i];
					string s1;
					ss >> s1;
					s = s + s1 + "=";
					if (i == 10 || i == 3) {
						stringstream ss;
						ss << sale[k][i].num;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else if (i == 2) {
						stringstream ss;
						ss << sale[k][i].val;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else {
						s += sale[k][i].s;
					}
					s += ";";
				}
				s += "|";
				boost::asio::write(socket, boost::asio::buffer(s), ignored);
			}
			ss << buy_num;
			ss >> s;
			boost::asio::write(socket, boost::asio::buffer(s), ignored);
			for (int k = 0; k < buy_num; k++) {
				string s = "";
				for (int i = 0; i < 15; i++) {
					stringstream ss;
					ss << a[i];
					string s1;
					ss >> s1;
					s = s + s1 + "=";
					if (i == 10 || i == 3) {
						stringstream ss;
						ss << buy[k][i].num;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else if (i == 2) {
						stringstream ss;
						ss << buy[k][i].val;
						string s1;
						ss >> s1;
						s = s + s1;
					}
					else {
						s += buy[k][i].s;
					}
					s += ";";
				}
				s += "|";
				boost::asio::write(socket, boost::asio::buffer(s), ignored);
			}
		}
	}	
	return 0;
}
