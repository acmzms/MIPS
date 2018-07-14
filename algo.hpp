#ifndef ALGORITHMS
#define ALGORITHMS
#include<vector>
#include<map>
#include<algorithm>
#include<iostream>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
struct line
{
    int type;
    vector<int> cont;
	line() :type(0) { vector<int> vi; cont = vi; }
    line(int a, vector<int> b): type(a), cont(b){}
};

int stringtot(string s)
{
    stringstream sst(s);
    int x;
    sst>>x;
    return x;
}

int registoint(string s)
{
    //if(s[0] != '$') return stringtot<int>(s);
    stringstream sst(s);
    char c;
    sst>>c;
    string x;
    sst>>x;
    if(x == "zero") return 0;
    if(x == "at") return 1;
    if(x == "v0") return 2;
    if(x == "v1") return 3;
    if(x == "a0") return 4;
    if(x == "a1") return 5;
    if(x == "a2") return 6;
    if(x == "a3") return 7;
    if(x == "t0") return 8;
    if(x == "t1") return 9;
    if(x == "t2") return 10;
    if(x == "t3") return 11;
    if(x == "t4") return 12;
    if(x == "t5") return 13;
    if(x == "t6") return 14;
    if(x == "t7") return 15;
    if(x == "s0") return 16;
    if(x == "s1") return 17;
    if(x == "s2") return 18;
    if(x == "s3") return 19;
    if(x == "s4") return 20;
    if(x == "s5") return 21;
    if(x == "s6") return 22;
    if(x == "s7") return 23;
	if(x == "t8") return 24;
    if(x == "t9") return 25;
    if(x == "gp") return 28;
    if(x == "sp") return 29;
    if(x == "fp") return 30;
    if(x == "ra") return 31;
    if(x == "hi") return 32;
    if(x == "lo") return 33;
    if(x == "pc") return 34;
    else return stringtot(x);
}

string process(string str)
{
    string ret;
    for(size_t i = 1;i < str.size() - 1;i++)
    {
        if(str[i] == '\\')
        {
            i++;
            if(str[i] == '\\')
            {
                ret += '\\';
            }
            if(str[i] == '\"')
            {
                ret += '\"';
            }
            if(str[i] == 'n')
            {
                ret += '\n';
            }
            if(str[i] == 't')
            {
                ret += '\t';
            }
            if(str[i] == 'r')
            {
                ret += '\r';
            }
            if(str[i] == '0')
            {
                ret += '\0';
            }
        }
        else
        {
            ret += str[i];
        }
    }
    return ret;
}

int separate(string &str)
{
	stringstream sts(str);
	char c;
	sts >> c;
	string s;
	while (c != '(')
	{
		s.push_back(c);
		sts >> c;
		//cout << '*';
	}
	int ret = stringtot(s);
	string rem;
	sts >> rem;
	rem.pop_back();
	str = rem;
	return ret;
}
#endif
