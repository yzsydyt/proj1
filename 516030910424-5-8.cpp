#include "std_lib_facilities.h"
char a[1000000][15];
int n,m;
int b[100000];
char q[1000000][15];
int sum[1000000];
int start[1000000];
int judge(char a[15], char b[15]) {
	for (int i = 0; i < n; i++) {
		if (a[i] < b[i]) { return 0; }
		if (a[i] > b[i]) { return 2; }
	}
	return 1;
}
void q_sort(int l, int r) {
	int i = l;
	int j = r;
	char v1[15];
	for (int i = 0; i < n; i++) { v1[i] = a[(l + r) / 2][i]; }
	int v = b[(l + r) / 2];
	while (i <= j) {
		while ((judge(a[i], v1) == 0) || (judge(a[i], v1) == 1) && (b[i] < v)) { i += 1; }
		while ((judge(a[j], v1) == 2) || (judge(a[j], v1) == 1) && (b[j] > v)) { j -= 1; }
		if (i <= j) {
			for (int v = 0; v < n; v++) { a[i][v], a[j][v] = a[j][v], a[i][v]; }
			b[i], b[j] = b[j], b[i];
			i += 1;
			j -= 1;
		}
	}
	if (i < r) { q_sort(i, r); }
	if (l < j) { q_sort(l, j); }
}
void qsort(int l, int r) {
	int i = l;
	int j = r;
	int v = sum[(i + j) / 2];
	while (i <= j) {
		while (sum[i] > v) { i += 1; }
		while (sum[j] < v) { j -= 1; }
		if (i <= j) {
			sum[i], sum[j] = sum[j], sum[i];
			for (int x = 0; x < n; x++) { q[i][x], q[j][x] = q[j][x], q[i][x]; }
			start[i], start[j] = start[j], start[i];
			i += 1;
			j -= 1;
		}
	}
	if (i < r) { qsort(i, r); }
	if (l < j) { qsort(l, j); }
}
int main() {
	cin >> n;
	char c[100];
	m = -1;
	int k = 0;
	ifstream f("EnglishWords.txt");
	while (!f.eof()) {
		f.getline(c, 100);
		int j = 0;
		while (c[j + 1] >= 'a'&&c[j + 1] <= 'z') { j += 1; }
		if (j >= n - 1) {
			m += 1;
			for (int i = 0; i < n; i++) {
				a[m][i] = c[j - n + i + 1];
			}
			b[m] = k;
		}
		k += 1;
	}
	f.close();
	q_sort(0, m);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < n; j++) { cout << a[i][j]; }
		cout << endl;
	}
	k = 0;
	start[0] = 0;
	for (int i = 0; i < n; i++) { q[0][i] = a[0][i]; }
	for (int i = 1; i <= m; i++) {
		if (judge(a[i], a[i - 1]) != 1) {
			sum[k] = i - start[k];
			k += 1;
			for (int j = 0; j < n; j++) { q[k][j] = a[i][j]; }
			start[k] = i;
		}
	}
	sum[k] = m - start[k] + 1;
	qsort(0, k);
	cout << k << endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < n; j++) { cout << q[i][j]; }
		cout << ' ' << sum[i] << endl;
	}
	string s;
	cin >> s;
	for (int i = 0; i < n; i++) { a[m + 1][i] = s[i]; }
	int i = 0;
	while (i <= k&&judge(a[m + 1], q[i]) != 1) { i += 1; }
	if (i > k) {
		cout << '0' << endl;
		return 0;
	}
	ifstream ff("EnglishWords.txt");
	i = start[i];
	cout << sum[i]<<endl;
	for (int j = 0; j < m; j++) {
		ff.getline(c, 100);
		if (j == b[i]) { 
			cout << c << endl;
			i += 1;
		}
	}
	ff.close();
	return 0;
}