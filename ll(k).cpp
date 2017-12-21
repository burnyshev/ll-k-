#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <stdlib.h>
using namespace std;

int k;
char start;
set <char> ne, te;//spisok neterminalov/terminalov
map< char, set<string>> rule;//pravila
map<char,set<string>> first_sym;
typedef struct {
	string u;
	char neterm;
	string str;
	string mnoz;
} LL2;
typedef struct{
	char neterminal;
	string l;
}LL1;
map<LL1, set<LL2>> TAB;
set<string> operator+(set<string> s1, set<string> s2) {
	set<string> result;
	for (auto i : s1) {
		for (auto l : s2) {
			string w;
			if (i == "e") {
				w = l;
			}
			else
				if (l == "e"){
					w = i;
				}
				else {
					w = i + l;
				}
			result.insert(w.substr(0, k));
		}
		if (s2.empty()) {
			result.insert(i.substr(0, k));
		}
	}
	if (s1.empty()) {
		for (auto l : s2) {
			result.insert(l.substr(0, k));
		}
	}
	return result;
}
//comparator for LL1
bool operator < (LL1 q, LL1 w){
	if (q.neterminal < w.neterminal){
		return true;
	} else {
		if (q.neterminal == w.neterminal){
			if (q.l < w.l){
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
}
bool operator ==(LL1 q, LL1 w){
	if (q.neterminal == w.neterminal && q.l == w.l){
		return true;
	} else {
		return false;
	}
}
bool operator < (LL2 q, LL2 w){
	if (q.u < w.u){
		return true;
	} else { 
		if (q.u > w.u ){
			return false;
		} else {
			if (q.neterm < w.neterm){
				return true;
			} else {
				if (q.neterm > w.neterm){
					return true;
				} else {
					if (q.neterm < w.neterm){
						return false;
					} else {
						if (q.str > w.str){
							return true;
						} else {
							if	(q.str < w.str){
								return false;
							} else {
								if (q.mnoz < q.mnoz){
									return true;
								} else {
									return false;
								}
							} 
						}  
					}
				}
			}
		}
	}
}
bool operator ==(LL2 q, LL2 w){
	if (q.mnoz == w.mnoz && q.neterm == w.neterm && q.str == w.str && q.u == w.u){
		return true;
	} else {
		return false;
	}
}
//FIRST для N и T
map<char,set<string>> First_NT() {
	map<char, set<string>> result0;
	map<char, set<string>> result1;
	for (auto i : te) {// 1 пункт
		result1[i].insert(string(1,i));
	}
	for (auto i : ne) {//2 пункт
		for (auto l : rule[i]) {
			string l1 = l.substr(0, k);
			bool check = false;
			for (int m = 0; m < (int)l1.length(); m++){
				if (ne.find(l1[m]) != ne.end()) {
					check = true;
					break;
				}
			}
			if (!check) {
				result1[i].insert(l1);
			}
		}
	}
	do { // 3 пункт
		result0 = result1;
		for (auto i : ne) {
			for (auto l : rule[i]) {
				set<string> r_sum;
				for (int m = 0; i < (int)l.length(); i++) {
					r_sum = r_sum + result0[l[m]];
				}
				result1[i].insert(r_sum.begin(), r_sum.end());
			}
		}
	} while (result0 != result1);
	return result1;
}
//FIRST для строки
set<string> FIRST_STR(string s){

	set<string> st;
	for (int i = 0; i< (int)s.length(); i++){
		st = st + first_sym[s[i]];
	}
	return st;
}

//построение LL(k)-таблиц
map<LL1, set<LL2>> LLTAB(){
	map <LL1, set<LL2>> m1;
	map <LL1, set<LL2>> m2;
	for (auto i: rule['S']){//1-2  пункт
		for (auto j: FIRST_STR(i)){
			for (int r = 0; r < (int)i.size(); r++){
				  if (ne.find(i[r]) != ne.end()){
					  m1[{'S', "e"}].insert({j, 'S', i, i.substr(r)});
					  break;
				  }
				  if (ne.find(i[r]) == ne.end()){
					  m1[{'S', "e"}].insert({j, 'S', i, "error"});
				  }
			}
		}
	}
	do {// 3 пункт
		for (auto i :m1){
			for (auto j: i.second){
				
			}
		}
		m1 = m2;
	} while (m1 != m2);

	return m1;
}

// ввод всех данных
void vvod(){
	cout << "Введите k" << endl;
	cin >> k;
	string net, ter;//terminaly i neterminaly
	cout << "Введи нетерминалы " << endl;
	cin >> net;
	for (int i = 0; i < (int)net.size(); i++){
		if ((int)net[i] >= 65 && (int)net[i]<= 90){
			ne.insert(net[i]);
		}
	}
	cout << "Введи терминалы " << endl;
	cin >> ter;
	for (int i = 0; i < (int)ter.size(); i++){
		if ((int)ter[i] >= 97 && (int)ter[i]<= 122 && (int)ter[i]!= 101){
			te.insert(ter[i]);
		}
	}

	cout << "Начальный нетерминал :" << endl;
	cin >> start;
	if (ne.find(start) == ne.end()){
		cout << "Такого нетерминала нет";
		exit(0);
	}

	int n;
	cout << "Введи число правил" << endl;
	cin >> n;
	cout << "Введи правила" << endl;
	for (int i = 0; i < n; i++){
		string a;// одно правило
		cin >> a;
		if (ne.find(a[0]) == ne.end()){
			cout << "Error wrong rule" << endl;
			exit(0);
		} else {
			string str;
			for (int j = 3; j < (int)a.size(); j++){
				if (a[j] == '|' || a[j] == 'e'){
					if (str != ""){
						rule[a[0]].insert(str);
						str = "";
					} else {
						str = "e";
						rule[a[0]].insert(str);
						str = "";
					}
				} else {
					if (ne.find(a[j]) == ne.end() && te.find(a[j]) == te.end() ){
						cout << "Error wrong rule " << endl;
						exit(0);
					} else {
						str = str + a[j];
					}
				}
			}
			if (str != ""){
				rule[a[0]].insert(str);
			}
		}
	}
	return;
}

int main() {
	vvod();
	first_sym = First_NT();
	TAB = LLTAB();
	return 0;
}
