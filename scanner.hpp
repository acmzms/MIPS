#ifndef SCANNER
#define SCANNER
#include<cstdio>
#include<iostream>
#include<sstream>
#include"algo.hpp"
using namespace std;
extern vector<line> code;
bool readline(ifstream& fs)
{
    string tot;
    char c = fs.get();
    if(c == EOF) return 0;
    while(c != '\n')
    {
        tot += c;
        c = fs.get();
    }
    stringstream ss(tot);
    string typ;
    ss>>typ;
	if (typ == ".ascii" || typ == ".asciiz")
	{
		string rem;
		string obt;
		while (ss >> obt)
		{
			rem.append(obt);
			rem.push_back(' ');
		}
		rem.pop_back();
		vector<string> vs;
		vs.push_back(rem);
		line l(typ, vs);
		code.push_back(l);
		return 1;
	}
    vector<string> vs;
    string tmp;
    while(ss >> tmp)
    {
		if (tmp.back() == ',') tmp.pop_back();
        vs.push_back(tmp);
    }
    line l(typ, vs);
    code.push_back(l);
    return 1;
}
#endif
