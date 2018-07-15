#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include"lexer.hpp"
#include"algo.hpp"
#include"scanner.hpp"
#include<vector>
#include<map>
#include<algorithm>
#include <cstring>
using namespace std;
enum regis
{
	ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, S0,
	S1, S2, S3, S4, S5, S6, S7, T8, T9, GP, SP, FP, RA, HI, LO, PC
};
int branch = 0;
vector<line> code;
char memory[4194312] = { 0 };
int registers[36] = { 0 };
bool reglock[36] = { 0 };
map<string, int> labels;
map<string, int> pointers;
vector<int> labval;
vector<int> ptrval;
int heapptr = 0;
int state = 0;
int curline = 1, endline, nextline;
line fetch()
{
	line k;
	if (curline > endline) return k;
	if (branch == -1)
	{
		line l;
		//cout << "command: " << l.type << "\tcurline: " << -1 << '\n';
		return l;
	}
	else if(branch == 0)
	{
		curline++;
		line l = code[curline];
		//cout << "command: " << l.type << "\tcurline: " << curline  << '\n';
		//if (curline == 469)
			//cout << ' ';
		nextline = curline;
		return l;
	}
	else
	{
		curline = branch;
		line l = code[curline];
		branch = 0;
		//cout << "command: " << l.type << "\tcurline: " << curline << '\n';
		nextline = curline + 1;
		return l;
	}
}
line decode(line l)
{
	line ret;
	int typ = l.type;
	//cout << "decode: " << typ << '\t';
	if (l.type == 0) return l;
	ret.type = l.type;
	if (typ == SYSCALL)
	{
		if (reglock[2] || reglock[3] || reglock[4])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		else
		{
			reglock[2] = 1;
			int com = registers[2];
			ret.cont.push_back(com);
			return ret;
		}
	}
	if (typ == ADD || typ == ADDU || typ == ADDIU || typ == SUB || typ == SUBU || typ == XOR || typ == XORU || typ == REM || typ == REMU || typ == SEQ || typ == SGE || typ == SGT || typ == SLE || typ == SLT || typ == SNE)
	{
		if (l.cont[0] == 1)
		{
			int d0 = l.cont[1];
			int d1 = l.cont[2];
			int d2 = l.cont[3];
			if (reglock[d1] || reglock[d2])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			reglock[d0] = 1;
			int r1 = registers[d1];
			int r2 = registers[d2];
			ret.cont.push_back(d0);
			ret.cont.push_back(r1);
			ret.cont.push_back(r2);
			return ret;
		}
		else
		{
			int d0 = l.cont[1];
			int d1 = l.cont[2];
			int d2 = l.cont[3];
			if (reglock[d1])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			reglock[d0] = 1;
			int r1 = registers[d1];
			int r2 = d2;
			ret.cont.push_back(d0);
			ret.cont.push_back(r1);
			ret.cont.push_back(r2);
			return ret;
		}
	}
	if (typ == NEG || typ == NEGU || typ == MOVE)
	{
		int d = l.cont[0];
		int s = l.cont[1];
		if (reglock[s])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		reglock[d] = 1;
		int r = registers[s];
		ret.cont.push_back(d);
		ret.cont.push_back(r);
		return ret;
	}
	if (typ == MFHI)
	{
		int d = l.cont[0];
		if (reglock[32])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		reglock[d] = 1;
		line k;
		k.type = MOVE;
		k.cont.push_back(d);
		k.cont.push_back(registers[32]);
		return k;
	}
	if (typ == MFLO)
	{
		int d = l.cont[0];
		if (reglock[33])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		reglock[d] = 1;
		line k;
		k.type = MOVE;
		k.cont.push_back(d);
		k.cont.push_back(registers[33]);
		return k;
	}
	if (typ == LI)
	{
		int d = l.cont[0];
		int imm = l.cont[1];
		reglock[d] = 1;
		line k;
		k.type = LI;
		k.cont.push_back(d);
		k.cont.push_back(imm);
		return k;
	}
	if (typ == MUL || typ == DIV || typ == MULU || typ == DIVU)
	{
		if (l.cont.size() == 4)
		{
			if (l.cont[0] == 1)
			{
				int d0 = l.cont[1];
				int d1 = l.cont[2];
				int d2 = l.cont[3];
				if (reglock[d1] || reglock[d2])
				{
					branch = curline;
					ret.type = 0;
					return ret;
				}
				reglock[d0] = 1;
				int r1 = registers[d1];
				int r2 = registers[d2];
				ret.cont.push_back(d0);
				ret.cont.push_back(r1);
				ret.cont.push_back(r2);
				return ret;
			}
			else
			{
				int d0 = l.cont[1];
				int d1 = l.cont[2];
				int d2 = l.cont[3];
				if (reglock[d1])
				{
					branch = curline;
					ret.type = 0;
					return ret;
				}
				reglock[d0] = 1;
				ret.type = typ;
				int r1 = registers[d1];
				int r2 = d2;
				ret.cont.push_back(d0);
				ret.cont.push_back(r1);
				ret.cont.push_back(r2);
				return ret;
			}
		}
		if (l.cont[0] == 1)
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			if (reglock[d1] || reglock[d2])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			reglock[32] = 1;
			reglock[33] = 1;
			int r1 = registers[d1];
			int r2 = registers[d2];
			ret.cont.push_back(r1);
			ret.cont.push_back(r2);
			return ret;
		}
		else
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			if (reglock[d1] || reglock[d2])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			reglock[32] = 1;
			reglock[33] = 1;
			int r1 = registers[d1];
			int r2 = d2;
			ret.cont.push_back(r1);
			ret.cont.push_back(r2);
			return ret;
		}
	}
	if (typ == LA || typ == LB || typ == LH || typ == LW || typ == SB || typ == SH || typ == SW)
	{
		int d = l.cont[0];
		bool flag = false;
		if(typ == LA || typ == LB || typ == LH || typ == LW)
		{
			ret.cont.push_back(d);
			flag = true;
		}
		if(typ == SB || typ == SH || typ == SW)
		{
			if (reglock[d])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			ret.cont.push_back(registers[d]);
		}
		int add;
		if (l.cont.size() == 3)
		{
			int x1 = l.cont[1];
			int x2 = l.cont[2];
			if (reglock[x2])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			int x3 = registers[x2];
			ret.cont.push_back(x1);
			ret.cont.push_back(x3);
		}
		else
		{
			add = ptrval[l.cont[1]];
			ret.cont.push_back(add);
		}
		if (flag) reglock[d] = 1;
		return ret;
	}
	if (typ == B || typ == J || typ == JAL)
	{
		int pos = labval[l.cont[0]];
		if(typ == JAL) ret.cont.push_back(nextline);
		branch = pos;
		return ret;
	}
	if (typ == JR || typ == JALR)
	{
		int d = l.cont[0];
		if (reglock[d])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		int pos = registers[d];
		if(typ == JALR) ret.cont.push_back(nextline);
		branch = pos;
		return ret;
	}
	if (typ == BEQ || typ == BNE || typ == BGE || typ == BLE || typ == BGT || typ == BLT)
	{
		int s = l.cont[0];
		int d1 = l.cont[1];
		int d2 = l.cont[2];
		if (s == 1)
		{
			if (reglock[d1] || reglock[d2])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			ret.cont.push_back(registers[d1]);
			ret.cont.push_back(registers[d2]);
		}
		else
		{
			if (reglock[d1])
			{
				branch = curline;
				ret.type = 0;
				return ret;
			}
			ret.cont.push_back(registers[d1]);
			ret.cont.push_back(d2);
		}
		branch = -1;
		int pos = labval[l.cont[3]];
		ret.cont.push_back(pos);
		return ret;
	}
	if (typ == BEQZ || typ == BNEZ || typ == BGEZ || typ == BLEZ || typ == BGTZ || typ == BLTZ)
	{
		int d = l.cont[0];
		if (reglock[d])
		{
			branch = curline;
			ret.type = 0;
			return ret;
		}
		branch = -1;
		int r = registers[d];
		int pos = labval[l.cont[1]];
		ret.cont.push_back(r);
		ret.cont.push_back(pos);
		return ret;
	}
	line k;
	return k;
}
line exec(line l)
{
	line ret;
	int typ = l.type;
	//cout << "execute: " << typ << '\t';
	if (typ == 0 || typ == SYSCALL) return l;
	ret.type = l.type;
	if (typ == ADD)
	{
		int res = l.cont[1] + l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == ADDU || typ == ADDIU)
	{
		unsigned int res = l.cont[1] + l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SUB)
	{
		int res = l.cont[1] - l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SUBU)
	{
		unsigned int res = l.cont[1] - l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == XOR)
	{
		int res = l.cont[1] ^ l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == XORU)
	{
		unsigned int res = l.cont[1] ^ l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == REM)
	{
		int res = l.cont[1] % l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == REMU)
	{
		unsigned int res = l.cont[1] % l.cont[2];
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SEQ)
	{
		int res = (l.cont[1] == l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SGE)
	{
		int res = (l.cont[1] >= l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SGT)
	{
		int res = (l.cont[1] > l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SLE)
	{
		int res = (l.cont[1] <= l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SLT)
	{
		int res = (l.cont[1] < l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == SNE)
	{
		int res = (l.cont[1] != l.cont[2]);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(res);
		return ret;
	}
	if (typ == NEG)
	{
		int d = l.cont[0];
		int r = -l.cont[1];
		ret.cont.push_back(d);
		ret.cont.push_back(r);
		return ret;
	}
	if (typ == NEGU)
	{
		int d = l.cont[0];
		unsigned int r = ~l.cont[1];
		ret.cont.push_back(d);
		ret.cont.push_back(r);
		return ret;
	}
	if (typ == MOVE || typ == LI)
	{
		return l;
	}
	if (typ == MUL)
	{
		if (l.cont.size() == 3)
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			int res = d1 * d2;
			ret.cont.push_back(1);
			ret.cont.push_back(l.cont[0]);
			ret.cont.push_back(res);
		}
		else
		{
			int d1 = l.cont[0];
			int d2 = l.cont[1];
			int res1 = (d1 * d2) / 4294967296;
			int res2 = (d1 * d2) % 4294967296;
			ret.cont.push_back(0);
			ret.cont.push_back(res1);
			ret.cont.push_back(res2);
		}
		return ret;
	}
	if (typ == MULU)
	{
		if (l.cont.size() == 3)
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			unsigned int res = d1 * d2;
			ret.cont.push_back(1);
			ret.cont.push_back(l.cont[0]);
			ret.cont.push_back(res);
		}
		else
		{
			int d1 = l.cont[0];
			int d2 = l.cont[1];
			unsigned int res1 = (d1 * d2) / 4294967296;
			unsigned int res2 = (d1 * d2) % 4294967296;
			ret.cont.push_back(0);
			ret.cont.push_back(res1);
			ret.cont.push_back(res2);
		}
		return ret;
	}
	if (typ == DIV)
	{
		if (l.cont.size() == 3)
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			int res = d1 / d2;
			ret.cont.push_back(1);
			ret.cont.push_back(l.cont[0]);
			ret.cont.push_back(res);
		}
		else
		{
			int d1 = l.cont[0];
			int d2 = l.cont[1];
			int res1 = (d1 / d2);
			int res2 = (d1 % d2);
			ret.cont.push_back(0);
			ret.cont.push_back(res1);
			ret.cont.push_back(res2);
		}
		return ret;
	}
	if (typ == DIVU)
	{
		if (l.cont.size() == 3)
		{
			int d1 = l.cont[1];
			int d2 = l.cont[2];
			unsigned int res = d1 / d2;
			ret.cont.push_back(1);
			ret.cont.push_back(l.cont[0]);
			ret.cont.push_back(res);
		}
		else
		{
			int d1 = l.cont[0];
			int d2 = l.cont[1];
			unsigned int res1 = (d1 / d2);
			unsigned int res2 = (d1 % d2);
			ret.cont.push_back(0);
			ret.cont.push_back(res1);
			ret.cont.push_back(res2);
		}
		return ret;
	}
	if (typ == LA || typ == LB || typ == LH || typ == LW || typ == SB || typ == SH || typ == SW)
	{
		int d = l.cont[0];
		ret.cont.push_back(d);
		int add;
		if (l.cont.size() == 3)
		{
			int x1 = l.cont[1];
			int x2 = l.cont[2];
			add = x1 + x2;
			ret.cont.push_back(add);
		}
		else
		{
			add = l.cont[1];
			ret.cont.push_back(add);
		}
		return ret;
	}
	if (typ == B || typ == J || typ == JR || typ == JAL || typ == JALR) return l;
	if (typ == BEQ)
	{
		if (l.cont[0] == l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BNE)
	{
		if (l.cont[0] != l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BGE)
	{
		if (l.cont[0] >= l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BLE)
	{
		if (l.cont[0] <= l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BGT)
	{
		if (l.cont[0] > l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BLT)
	{
		if (l.cont[0] < l.cont[1])
		{
			branch = l.cont[2];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BEQZ)
	{
		if (l.cont[0] == 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BNEZ)
	{
		if (l.cont[0] != 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BLEZ)
	{
		if (l.cont[0] <= 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BGEZ)
	{
		if (l.cont[0] >= 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BGTZ)
	{
		if (l.cont[0] > 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	if (typ == BLTZ)
	{
		if (l.cont[0] < 0)
		{
			branch = l.cont[1];
			ret.cont.push_back(branch);
			return ret;
		}
		else
		{
			branch = 0;
			line k;
			return k;
		}
	}
	line k;
	return k;
}
line memaccess(line l)
{
	line ret;
	int typ = l.type;
	//cout << "memory:" << typ << '\t';
	if (l.type == 0) return l;
	ret.type = l.type;
	if (typ == SYSCALL)
	{
		reglock[2] = 0;
		int r = syscall(l.cont[0]);
		//cout << "syscall: " << l.cont[0] << '\t';
		if (r == 0)
		{
			//system("PAUSE");
			exit(0);
		}
		ret.type = 0;
		return ret;
	}
	if (typ == ADD || typ == ADDU || typ == ADDIU || typ == SUB || typ == SUBU || typ == XOR || typ == XORU || typ == REM || typ == REMU || typ == SEQ || typ == SGE || typ == SGT || typ == SLE || typ == SLT || typ == SNE)
	{
		return l;
	}
	if (typ == NEG || typ == NEGU || typ == MOVE || typ == MUL || typ == MULU || typ == DIV || typ == DIVU || typ == LA || typ == B || typ == J || typ == JAL || typ == JR || typ == JALR || typ == LI)
	{
		return l;
	}
	if (typ == BEQ || typ == BNE || typ == BGE || typ == BLE || typ == BGT || typ == BLT || typ == BEQZ || typ == BNEZ || typ == BGEZ || typ == BLEZ || typ == BGTZ || typ == BLTZ)
	{
		return l;
	}
	if (typ == LB)
	{
		int a = 0;
		int add = l.cont[1];
		memcpy(&a, &memory[add], 1);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(a);
		return ret;
	}
	if (typ == LH)
	{
		int a = 0;
		int add = l.cont[1];
		memcpy(&a, &memory[add], 2);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(a);
		return ret;
	}
	if (typ == LW)
	{
		int a = 0;
		int add = l.cont[1];
		memcpy(&a, &memory[add], 4);
		ret.cont.push_back(l.cont[0]);
		ret.cont.push_back(a);
		return ret;
	}
	if (typ == SB)
	{
		int a = 0;
		a = l.cont[0];
		int add = l.cont[1];
		memcpy(&memory[add], &a, 1);
	}
	if (typ == SH)
	{
		int a = 0;
		a = l.cont[0];
		int add = l.cont[1];
		memcpy(&memory[add], &a, 2);
	}
	if (typ == SW)
	{
		int a = 0;
		a = l.cont[0];
		int add = l.cont[1];
		memcpy(&memory[add], &a, 4);
	}
	return l;
}
void writereg(line l)
{
	line ret;
	int typ = l.type;
	//cout << "write: " << typ << '\t';
	if (l.type == 0) return;
	if (typ == ADD || typ == ADDU || typ == ADDIU || typ == SUB || typ == SUBU || typ == XOR || typ == XORU || typ == REM || typ == REMU || typ == SEQ || typ == SGE || typ == SGT || typ == SLE || typ == SLT || typ == SNE)
	{
		int d = l.cont[0];
		registers[d] = l.cont[1];
		reglock[d] = 0;
		return;
	}
	if (typ == NEG || typ == NEGU || typ == MOVE || typ == LI)
	{
		int d = l.cont[0];
		registers[d] = l.cont[1];
		reglock[d] = 0;
		return;
	}
	if (typ == MUL || typ == DIV || typ == MULU || typ == DIVU)
	{
		int s = l.cont[0];
		if (s == 1)
		{
			int d = l.cont[1];
			registers[d] = l.cont[2];
			reglock[d] = 0;
			return;
		}
		registers[33] = l.cont[1];
		registers[32] = l.cont[2];
		reglock[32] = 0;
		reglock[33] = 0;
		return;
	}
	if (typ == LA || typ == LB || typ == LH || typ == LW)
	{
		int r = l.cont[0];
		int v = l.cont[1];
		registers[r] = v;
		reglock[r] = 0;
		return;
	}
	if (typ == JAL || typ == JALR)
	{
		registers[31] = l.cont[0] + 1;
	}

	/*FILE* fp = fopen("register.log", "a+");
	for (int i = 0; i < 35; ++i) {
		fprintf(fp, "register[%d] : %d ", i, registers[i]);
		if ((i + 1) % 5 == 0) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	fclose(fp);*/

}
int main(int argc, char *argv[])
{
	//ios::sync_with_stdio(false);
	//cin.tie(0);
	//cout.tie(0);
	//ifstream pre("test2.txt");
	//cout << boolalpha << fs.rdbuf()->is_open() << endl;
	//int ttt;
	//cin >> ttt;
	//if (!fs) throw;
	line xx;
	code.push_back(xx);
	registers[29] = 4194304;
	int runpos = 1, y = 1;
	ifstream fs(argv[1]);
	//ifstream fs("test2.txt");
	while (1)
	{
		y = readline(fs, state);
		if (y == 0) break;
		if (code[curline].type == MAIN)
		{
			runpos = curline;
		}
		if (code[curline].type == DATA) state = 0;
		if (code[curline].type == TEXT) state = 1;
		//cout << "command: " << code[curline].type << "\t\t\t\t";
		//cout << "curline: " << curline << '\n';
		curline++;
	}
	endline = curline;
	curline = runpos - 1;
	line ifret;
	line idret;
	line exret;
	line maret;
	while (1)
	{
		writereg(maret);
		maret = memaccess(exret);
		exret = exec(idret);
		idret = decode(ifret);
		ifret = fetch();
		/*FILE* fp = fopen("register.log", "a+");
		fprintf(fp, "Run Line %d: %d\n", ifret.type, (ifret.cont.size() == 0 ? -1 : ifret.cont[0]));
		fclose(fp);*/
	}
	fs.close();
	//system("PAUSE");
	//getchar();
	return 0;
}
