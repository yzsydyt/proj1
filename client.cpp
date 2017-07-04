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
token order[20];
int sale_num, buy_num;
int a[20];
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
	a[2] = 14;//val
	a[3] = 31;//num
	a[4] = 32;
	a[5] = 35;
	a[6] = 38;
	a[7] = 39;
	a[8] = 40;
	a[9] = 41;
	a[10] = 44;//num
	a[11] = 54;
	a[12] = 55;
	a[13] = 150;
	a[14] = 151;
}

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	string ip_str;
	cout << "Please input IP address:";
	cin >> ip_str;
	tcp::resolver::query query(ip_str, "1323");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	boost::system::error_code error;
	string idnumber = "";
	bool p = false;
	while (p==false) {
		std::cout << "login,register or quit?" << endl;
		string s;
		cin >> s;
		if (s == "login") {//login
			string id, password;
			std::cout << "id:";
			std::cin >> id;
			std::cout << "password:";
			std::cin >> password;
			boost::asio::write(socket, boost::asio::buffer("login"), error);
			boost::asio::write(socket, boost::asio::buffer(id), error);
			boost::asio::write(socket, boost::asio::buffer(password), error);
			std::array<char, 500> input_buffer;
			std::size_t rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			while (input_buffer[0] == 'f') {
				std::cout << "WRONG!!Please try again." << endl;
				std::cout << "id:";
				std::cin >> id;
				std::cout << "password:";
				std::cin >> password;
				boost::asio::write(socket, boost::asio::buffer(id), error);
				boost::asio::write(socket, boost::asio::buffer(password), error);
				std::size_t rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
			}
			std::cout << "SUCCESS" << endl;
			for (int i = 0; i < rsize; i++) {
				idnumber += input_buffer[i];
			}
			p = true;
		}
		else if (s == "register") {//register
			string id, password;
			boost::asio::write(socket, boost::asio::buffer("register"), error);
			std::cout << "id:";
			std::cin >> id;
			boost::asio::write(socket, boost::asio::buffer(id), error);
			std::array<char, 500> input_buffer;
			std::size_t rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			while (input_buffer[0] == 'r') {
				cout << "Your id was registered.Please try a new id." << endl;
				std::cout << "id:";
				std::cin >> id;
				boost::asio::write(socket, boost::asio::buffer(id), error);
				size_t rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
			}
			std::cout << "password:";
			std::cin >> password;
			boost::asio::write(socket, boost::asio::buffer(password), error);
			rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			for (int i = 0; i < rsize; i++) {
				idnumber += input_buffer[i];
			}
			p = true;
		}
		else if (s == "quit") { return 0; }
		else { std::cout << "Invalid input"; }
	}
	while (true) {
		cout << "send buy order or sale order?(buy/sale)" << endl;
		string s;
		cin >> s;
		if (s == "buy" || s == "sale") {
			for (int i = 0; i < 15; i++) {
				order[i].val = 0;
				order[i].num = 0;
				order[i].s = "";
			}
			if (s == "buy") { order[11].s = "1"; }
			else { order[11].s = "2"; }
			order[9].s = idnumber;
			cout << "price:";
			double x;
			cin >> x;
			order[10].num = x;
			int n;
			cout << "num:";
			cin >> n;
			order[2].val = n;
			string s = "";
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
			boost::asio::write(socket, boost::asio::buffer(s), error);
			std::array<char, 500> input_buffer;
			std::size_t rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			char c[500];
			for (int i = 0; i < rsize; i++) { c[i] = input_buffer[i]; }
			inputwork(c);
			if (order[9].s == idnumber&&order[5].s == "8") {
				cout << "Your order has been submitted successfully." << endl;
				std::array<char, 500> input_buffer;
				std::size_t rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
				char c[500];
				for (int i = 0; i < rsize; i++) { c[i] = input_buffer[i]; }
				inputwork(c);
				if (order[13].s == "1") { cout << "Partial fill" << endl; }
				else { cout << "Full fill" << endl; }
			}
			else { cout << "Your order unfortunately hasn't been submitted" << endl; }
		}
	}
	return 0;
}