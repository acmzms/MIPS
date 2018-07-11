#ifndef SCANNER
#define SCANNER
#include<cstdio>
#include<iostream>
#include<sstream>
#include"algo.hpp"
using namespace std;
extern vector<line> code;
extern map<string, int> labels;
extern map<string, int> pointers;
extern vector<int> labval;
extern vector<int> ptrval;
extern int curline, heapptr;
int curlabel = 0, curpointer = 0;
bool readline(ifstream& fs, int sys)
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
    ss >> typ;
	vector<string> vs;
	vector<int> vi;
	if (typ == ".ascii")
	{
		string tmp;
		string obt;
		while (ss >> obt)
		{
			tmp.append(obt);
			tmp += ' ';
		}
		tmp.pop_back();
		ascii(tmp);
		line l(typ, vi);
		code.push_back(l);
		return 1;
	}
	if (typ == ".asciiz")
	{
		string tmp;
		string obt;
		while (ss >> obt)
		{
			tmp.append(obt);
			tmp += ' ';
		}
		tmp.pop_back();
		asciiz(tmp);
		line l(typ, vi);
		code.push_back(l);
		return 1;
	}
    string tmp;
    while(ss >> tmp)
    {
		if (tmp.back() == ',') tmp.pop_back();
        vs.push_back(tmp);
    }
	if (typ != "" && typ[typ.size() - 1] == ':')
	{
		typ.pop_back();
		if (sys == 0)
		{
			if (pointers.find(typ) == pointers.end())
			{
				ptrval.push_back(heapptr);
				pointers[typ] = curpointer;
				curpointer++;
			}
			else
			{
				ptrval[pointers[typ]] = heapptr;
			}
		}
		if (sys == 1)
		{
			if (labels.find(typ) == labels.end())
			{
				labval.push_back(curline);
				labels[typ] = curlabel;
				curlabel++;
			}
			else
			{
				labval[labels[typ]] = curline;
			}
		}
	}
	if (typ == ".ascii")
	{
		ascii(vs[0]);
	}
	if (typ == ".asciiz")
	{
		asciiz(vs[0]);
	}
	if (typ == ".align")
	{
		align(stringtot(vs[0]));
	}
	if (typ == ".space")
	{
		space(stringtot(vs[0]));
	}
	if (typ == ".byte")
	{
		for (size_t i = 0; i < vs.size(); i++)
		{
			registers[35] = stringtot(vs[i]);
			memcpy(&memory[heapptr], &registers[35], 1);
			heapptr++;
		}
	}
	if (typ == ".half")
	{
		for (size_t i = 0; i < vs.size(); i++)
		{
			registers[35] = stringtot(vs[i]);
			memcpy(&memory[heapptr], &registers[35], 2);
			heapptr += 2;
		}
	}
	if (typ == ".word")
	{
		for (size_t i = 0; i < vs.size(); i++)
		{
			registers[35] = stringtot(vs[i]);
			memcpy(&memory[heapptr], &registers[35], 4);
			heapptr += 4;
		}
	}
	if (typ == "add" || typ == "addu" || typ == "addiu" || typ == "sub" || typ == "subu" || typ == "xor" || typ == "xoru" || typ == "rem" || typ == "remu" || typ == "seq" || typ == "sge" || typ == "sgt" || typ == "sle" || typ == "slt" || typ == "sne")
	{
		int d = registoint(vs[0]);
		int d1 = registoint(vs[1]);
		if (vs[2][0] == '$')
		{
			int d2 = registoint(vs[2]);
			vi.push_back(1);
			vi.push_back(d);
			vi.push_back(d1);
			vi.push_back(d2);
		}
		else
		{
			int imm = stringtot(vs[2]);
			vi.push_back(0);
			vi.push_back(d);
			vi.push_back(d1);
			vi.push_back(imm);
		}
	}
	if (typ == "neg" || typ == "negu" || typ == "move")
	{
		int d = registoint(vs[0]);
		int d1 = registoint(vs[1]);
		vi.push_back(d);
		vi.push_back(d1);
	}
	if (typ == "mfhi")
	{
		int d = registoint(vs[0]);
		vi.push_back(d);
		//vi.push_back(32);
	}
	if (typ == "mflo")
	{
		int d = registoint(vs[0]);
		vi.push_back(d);
		//vi.push_back(33);
	}
	if (typ == "li")
	{
		int d = registoint(vs[0]);
		int imm = stringtot(vs[1]);
		vi.push_back(d);
		vi.push_back(imm);
	}
	if (typ == "mul" || typ == "mulu" || typ == "div" || typ == "divu")
	{
		if (vs.size() == 3)
		{
			int d = registoint(vs[0]);
			int d1 = registoint(vs[1]);
			if (vs[2][0] == '$')
			{
				int d2 = registoint(vs[2]);
				vi.push_back(1);
				vi.push_back(d);
				vi.push_back(d1);
				vi.push_back(d2);
			}
			else
			{
				int imm = stringtot(vs[2]);
				vi.push_back(0);
				vi.push_back(d);
				vi.push_back(d1);
				vi.push_back(imm);
			}
		}
		else
		{
			int d1 = registoint(vs[0]);
			if (vs[1][0] == '$')
			{
				int d2 = registoint(vs[1]);
				vi.push_back(1);
				vi.push_back(d1);
				vi.push_back(d2);
			}
			else
			{
				int imm = stringtot(vs[1]);
				vi.push_back(0);
				vi.push_back(d1);
				vi.push_back(imm);
			}
		}
	}
	if (typ == "la" || typ == "lb" || typ == "lh" || typ == "lw" || typ == "sb" || typ == "sh" || typ == "sw")
	{
		int d = registoint(vs[0]);
		vi.push_back(d);
		string tmp = vs[1];
		if (tmp.back() == ')')
		{
			int dis = separate(tmp);
			int d1 = registoint(tmp);
			vi.push_back(dis);
			vi.push_back(d1);
		}
		else
		{
			int add;
			if (pointers.find(tmp) == pointers.end())
			{
				ptrval.push_back(-1);
				pointers[tmp] = curpointer;
				add = curpointer;
				curpointer++;
			}
			else
			{
				add = pointers[tmp];
			}
			vi.push_back(add);
		}
	}
	if (typ == "b" || typ == "j" || typ == "jal")
	{
		string lab = vs[0];
		int pos;
		if (labels.find(lab) == labels.end())
		{
			labval.push_back(-1);
			labels[lab] = curlabel;
			pos = curlabel;
			curlabel++;
		}
		else
		{
			pos = labels[lab];
		}
		vi.push_back(pos);
	}
	if (typ == "jr" || typ == "jalr")
	{
		int d = registoint(vs[0]);
		vi.push_back(d);
	}
	if (typ == "beq" || typ == "bne" || typ == "bge" || typ == "ble" || typ == "bgt" || typ == "blt")
	{
		int d1 = registoint(vs[0]);
		if (vs[1][0] == '$')
		{
			int d2 = registoint(vs[1]);
			vi.push_back(1);
			vi.push_back(d1);
			vi.push_back(d2);
		}
		else
		{
			int imm = stringtot(vs[1]);
			vi.push_back(0);
			vi.push_back(d1);
			vi.push_back(imm);
		}
		string lab = vs[2];
		int pos;
		if (labels.find(lab) == labels.end())
		{
			labval.push_back(-1);
			labels[lab] = curlabel;
			pos = curlabel;
			curlabel++;
		}
		else
		{
			pos = labels[lab];
		}
		vi.push_back(pos);
	}
	if (typ == "beqz" || typ == "bnez" || typ == "bgez" || typ == "blez" || typ == "bgtz" || typ == "bltz")
	{
		int d = registoint(vs[0]);
		string lab = vs[1];
		int pos;
		if (labels.find(lab) == labels.end())
		{
			labval.push_back(-1);
			labels[lab] = curlabel;
			pos = curlabel;
			curlabel++;
		}
		else
		{
			pos = labels[lab];
		}
		vi.push_back(d);
		vi.push_back(pos);
	}
	line l(typ, vi);
    code.push_back(l);
    return 1;
}
#endif
