#include <iostream>
#include <string>
#include <vector>
using namespace std;

void netlist(vector<vector<char>> y, vector<char> neg, vector<int>neg_wires, int &w, int &m, char output, int len, int old);

int main() {
	
	string input;
	cout << "Enter boolean expression: ";
	cin >> input;
	cout << "\nNetlist Code:\n";
	vector<char> neg;
	vector<int> neg_wires;
	int w = 1;
	int m = 1;
	int nw = 1;
	int old = 0;

	for (int k=0; k<input.length();k++){
		string str = "";
		
		while(input[k]!=';' && k!=input.length()){
			str += input[k];
			k++;
		}
		
		vector<vector<char>> y;
		vector<char> temp;
		y.push_back(temp);
		bool neg_flag = false;
		int count = 0;
		bool nfound = false;
		char output = str[0];
		
		old = neg.size();

		for (int i = 2; i < str.length(); i++) {
			char x = str[i];
			neg_flag = false;
			if (str[i + 1] != '\'') {
				neg_flag = true;
				nfound = false;
				if (neg.size() == 0) {
					neg.push_back(x);
					neg_wires.push_back(w++);
				}
				else {
					for (int j = 0; j < neg.size(); j++) {
						if (x == neg[j]) {
							nfound = true;
							nw = neg_wires[j];
							break;
						}
					}
					if (!nfound) {
						neg.push_back(x);
						neg_wires.push_back(w);
						nw = w;
						w++;
					}
				}
			}
			else {
				if ((str.length()) == 4) {
					neg.push_back(x);
					neg_wires.push_back(w);
				w++;
				}
				if ((i + 1) == str.length()) {
					neg.push_back(x);
					neg_wires.push_back(w++);
				}
				i++;
				neg_flag = false;
			}

			if ((i + 1) >= str.length()) break;
			if (str[i + 1] == '&') {

				if (i == 2 || i == 3) {
					if (neg_flag)
						y[0].push_back(static_cast<char>(nw));
					else
						y[0].push_back(x);
				}
				if (str[i + 3] != '\'')
					y[count].push_back(static_cast<char>(nw + 1));
				else
					y[count].push_back(str[i + 2]);
				i++;
			}

			else if (str[i + 1] == '|') {

				if (i == 2 || i == 3) {
					if (neg_flag)
						y[0].push_back(static_cast<char>(nw));
					else
						y[0].push_back(x);
				}

				vector<char> temp;
				if (str[i + 3] != '\'')
					temp.push_back(static_cast<char>(nw + 1));
				else
					temp.push_back(str[i + 2]);
				y.push_back(temp);
				count++;
				i++;
			}
		}
		int len = str.length();
		netlist(y, neg, neg_wires, w, m, output, len, old);

	}
	return 0;
}


void netlist(vector<vector<char>> y, vector<char> neg, vector<int>neg_wires, int &w, int &m, char output, int len, int old){

//inverters

	for (int i = old; i < neg.size(); i++) {
		char x = neg[i];
		cout << "M" << m << " ";
		m++;
	    if (m <= 10) cout << " ";
		if (len==4) cout << output;
		else cout << neg_wires[i];
		cout << " " << x << " vdd vdd" << " PMOS\n";
		
		cout << "M" << m << " ";
		m++;
		if (m <= 10) cout << " ";
		if (len==4) cout << output;
		else cout << neg_wires[i];
		cout << " " << x << "  0   0 " << " NMOS\n";
	}
	cout << endl;

//PUN

	bool found = false;

	for (int i = 0; i < y.size(); i++) {
		for (int j = 0; j < y[i].size(); j++) {

			//M#
			cout << "M" << m << " ";
			m++;
			if (m <= 10) cout << " ";

			//drain (bottom of transistor in PUN)
			if (j == y[i].size() - 1) {
				cout << output << " ";
			}
			else {
				if (j == 0)cout << w << " ";
				else cout << (w + 1) << " ";
			}

			//gate (input)
			if (y[i][j] < 65 || (y[i][j] > 90 && y[i][j] < 97) || y[i][j]>122)
				cout << '0' + y[i][j] - 48 << " ";
			else cout << y[i][j] << " ";

			//source and body (top of transistor)
			if (j == 0) {
				cout << "vdd vdd";
			}
			else {
				cout << " " << w << "   " << w << " ";
				w++;
			}

			//type
			cout << " PMOS\n";
		}
	}
	cout << endl;

//PDN

	int top = 0;;
	int bottom = w;

	for (int i = 0; i < y.size(); i++) {
		if (i == y.size() - 1) bottom = 0;
		for (int j = 0; j < y[i].size(); j++) {

			//M#
			cout << "M" << m << " ";
			m++;
			if (m <= 10) cout << " ";

			//drain (top of transistor)
			if (i == 0) {
				cout << output << " ";
			}
			else {
				cout << top << " ";
			}

			//gate (input)
			if (y[i][j] < 65 || (y[i][j] > 90 && y[i][j] < 97) || y[i][j]>122)
				cout << '0' + y[i][j] - 48 << " ";
			else cout << y[i][j] << " ";

			//source and body (bottom of transistor)
			cout << bottom << " " << bottom;

			//type
			cout << " NMOS\n";

		}
		w=top;
		w++;
		top = bottom;
		bottom++;

	}
	cout << endl;
	return;
}