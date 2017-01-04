#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
using namespace std;

class Node {
public:
	char ch;
	int count;
	Node (char c, int i): ch(c), count(i) {}
	void addone () {count++;}
	Node (const Node & no): ch(no.ch), count(no.count) {}
	
};

class TreeNode {
public:
	char ch;
	int freq;
	TreeNode * lt;
	TreeNode * rt;
	TreeNode (char c, int f): ch(c), freq(f), lt(NULL), rt(NULL) {}
//	TreeNode (char c, int f, TreeNode * l, TreeNode * r): ch(c), freq(f) {
//		if ()
//	}
	
	~TreeNode () {delete lt; delete rt;}
	TreeNode& operator = (const TreeNode& tn) {
		this -> ch = tn.ch;
		this -> freq = tn.freq;
		this -> lt = new TreeNode (*(tn.lt));
		this -> rt = new TreeNode (*(tn.rt));
		return *this;
	}
};

TreeNode* copynode (TreeNode* tn) {
	if (tn == NULL) {
		return NULL;
	}
	else {
		TreeNode * newnode = new TreeNode (tn -> ch, tn -> freq);
		newnode -> lt = copynode (tn -> lt);
		newnode -> rt = copynode (tn -> rt);
		return newnode;
	}
}

void readfromfile (ifstream & file, vector <Node> & vec) {
	char ch;
	while (file >> noskipws >> ch) {
//		cout << ch << endl;
//		if (file == EOF) {break;}

		bool b = false;
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i].ch == ch) {
				vec[i].addone();

				for (int j = i+1; j < vec.size(); j++) {
					if (vec[j].count >= vec[i].count) {
						vec.insert (vec.begin() + j, vec[i]);
						vec.erase (vec.begin() + i);
						b = true;
						break;
					}
				}
				if (b == false) {
					vec.insert (vec.end(), vec[i]);
					vec.erase (vec.begin() + i);
				}

				b = true;
				break;
			}
		}
		if (b == false) {
			Node temp(ch, 1);
			vec.insert (vec.begin(), temp);
		}
	}
}

void constructHuffmanTree (vector <Node> & vec, TreeNode * & root) {
	vector <TreeNode *> temp;
	for (int i = 0; i < vec.size(); i++) {
		TreeNode * tn = new TreeNode (vec[i].ch, vec[i].count);
		temp.push_back(tn);
	}

	while (temp.size() != 1) {
		TreeNode * tn = new TreeNode ('\0', temp[0]->freq + temp[1]->freq);
		tn -> lt = copynode (temp[0]);
		tn -> rt = copynode (temp[1]);
//cout << temp.size() << endl;
//for (int i = 0; i < temp.size(); i++) {cout << temp[i] -> freq << " ";}cout << endl;
		temp.erase (temp.begin(), temp.begin()+2);
//		temp.erase (temp.begin() + 1);
//for (int i = 0; i < temp.size(); i++) {cout << temp[i] -> freq << " ";}cout << endl;
		bool b = false;
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] -> freq >= tn -> freq) {
				temp.insert (temp.begin() + i, tn);
				b = true;
				break;
			}
		}
		if (b == false) {
			temp.insert (temp.end(), tn);
		}
//for (int i = 0; i < temp.size(); i++) {cout << temp[i] -> freq << " ";}cout << endl;
	}
	root = copynode(temp[0]);
}

void rightthrough (string str, string list[], TreeNode * & root);

void leftthrough (string str, string list[], TreeNode * & root) {
	if (root == NULL) {
		return;
	}
	else if (root -> lt == NULL && root -> rt == NULL) {
		str += "0";
		list[int(root -> ch)] = str;
		return;
	}
	else {
		str += "0";
		leftthrough (str, list, root -> lt);
		rightthrough (str, list, root -> rt);
	}
}

void rightthrough (string str, string list[], TreeNode * & root) {
	if (root == NULL) {
		return;
	}
	else if (root -> lt == NULL && root -> rt == NULL) {
		str += "1";
		list[int(root -> ch)] = str;
		return;
	}
	else {
		str += "1";
		leftthrough (str, list, root -> lt);
		rightthrough (str, list, root -> rt);
	}
}

void constructHuffmanList (string list[], TreeNode * & root) {
	if (root == NULL) {
		return;
	}
	else if (root -> lt == NULL && root -> rt == NULL) {
		list[int(root -> ch)] = "0";
	}
	else {
		string base;
		leftthrough (base, list, root -> lt);
		rightthrough (base, list, root -> rt);
	}
}

void print (string list[], int size) {
	for (int i = 0; i < size; i++) {
		cout << char(i) << " : " << list[i] << " ;" << endl;
	}
	cout << "end" << endl;
}

void print(TreeNode * & root) {
	if (root == NULL) {
		cout << "end" << endl;
	}
	else {
		cout << root -> freq << " ";
		print (root -> lt);
		print (root -> rt);
	}
}

void huffmancondense (ifstream & in, ofstream & out, string list[]) {
	char ch;
	int i = 0, count = 0;
	for (int j = 0; j < 256; j++) {
		if (list[j] == "") {
			continue;
		}
		count++;
	}
	out << count << endl;
	for (int j = 0; j < 256; j++) {
		if (list[j] == "") {
			continue;
		}
		out << char (j) << " "  << list[j] << endl;
	}
	out << endl;
	while (in >> noskipws >> ch) {
		out << list[int(ch)];
		i++;
		if(i % 8 == 0) {
			out << endl;
		}
	}
}

string tobinary (int n) {
	string str;
	while(1) {
		if (n == 1) {
			str = "1" + str;
			break;
		}
		else if (n % 2 == 0) {
			str = "0" + str;
			n = n/2;
		}
		else if (n % 2 == 1) {
			str = "1" + str;
			n = n/2;
		}
	}
	if (str.length() == 8) {
		return str;
	}
	
	else {
		int m = 8-str.length();
		for (int i = 0; i < m; i++) {
			str = "0" + str;
		}
		return str;
	}
}

void throwtofake (ifstream & in, ofstream & out) {
	char ch;
	int i = 0;
	while (in >> noskipws >> ch) {
		out << tobinary(int(ch));
		i++;
		if(i % 8 == 0) {
			out << endl;
		}
//		cout << int (ch) << " : " << tobinary(int(ch)) << endl;
	}
}

int main () {
	ifstream file;
	file.open ("test.txt", ios::in);
	vector <Node> result;
	readfromfile (file, result);
//	for (int i = 0; i < result.size(); i++) {
//		cout << result[i].ch << " " << result[i].count << endl;
//	}

	TreeNode * root;
	constructHuffmanTree (result, root);
//	print (root);

	string HuffmanList [256];
	constructHuffmanList (HuffmanList, root);
//	print(HuffmanList, 256);

	file.clear();
	file.seekg(0, ios::beg);
	ofstream output;
	output.open ("result.txt", ios::out);

	huffmancondense (file, output, HuffmanList);

	file.clear();
	file.seekg(0, ios::beg);
	ofstream output2;
	output2.open ("fakebinarytext.txt", ios::out);
	throwtofake (file, output2);
}
