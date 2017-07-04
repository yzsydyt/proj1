#include <array>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include"std_lib_facilities.h"
#include<time.h>
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
	clock_t last, now;
	last = clock();
	while (true) {
		now = clock();
		if (double(now - last) > 300) {
			last = now;
			boost::asio::write(socket, boost::asio::buffer("quest"), error);
			std::array<char, 500> input_buffer;
			std::size_t rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			int sale_num = 0;
			for (int i = 0; i < rsize; i++) { sale_num = sale_num * 10 + input_buffer[i] - '0'; }
			ofstream f("saleorder.txt");
			f << "the total number of sale order:";
			f << sale_num;
			f << endl;
			for (int i = 0; i < sale_num; i++) {
				rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
				char c[500];
				for (int j = 0; j < rsize; j++) { c[j] = input_buffer[j]; }
				inputwork(c);
				string s = "";
				s = s + "salerid:" + order[9].s + ";";
				s = s + "price:";
				stringstream ss;
				ss << order[10].num;
				string s1;
				ss >> s1;
				s = s + s1 + ";";
				s = s + "amount:";
				ss << order[2].val;
				ss >> s1;
				s = s + s1 + ";";
				f << s;
				f << endl;
			}
			f.close();
			ofstream ff("buyorder.txt");
			rsize = socket.read_some(
				boost::asio::buffer(input_buffer), error);
			int buy_num = 0;
			for (int i = 0; i < rsize; i++) { buy_num = buy_num * 10 + input_buffer[i] - '0'; }
			ff << "the total number of buy order:";
			ff << buy_num;
			ff << endl;
			for (int i = 0; i < buy_num; i++) {
				rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
				char c[500];
				for (int j = 0; j < rsize; j++) { c[j] = input_buffer[j]; }
				inputwork(c);
				string s = "";
				s = s + "buyerid:" + order[9].s + ";";
				s = s + "price:";
				stringstream ss;
				ss << order[10].num;
				string s1;
				ss >> s1;
				s = s + s1 + ";";
				s = s + "amount:";
				ss << order[2].val;
				ss >> s1;
				s = s + s1 + ";";
				ff << s;
				ff << endl;
			}
			ff.close();
		}
	}
	return 0;
}