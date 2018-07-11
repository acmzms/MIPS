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
extern enum command;
vector<line> code;
char memory[4194312] = { 0 };
int registers[36] = { 0 };
map<string, int> labels;
map<string, int> pointers;
vector<int> labval;
vector<int> ptrval;
int heapptr = 0;
int state = 0;
int curline = 1;
int main(int argc, char *argv[])
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
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
	int endline = curline;
	curline = runpos;
	while (curline < endline)
	{
		if (code[curline].type == ADD)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] + registers[d2];
			if (op == 0) registers[d] = registers[d1] + d2;
		}
		if (code[curline].type == ADDU)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] + registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] + d2);
		}
		if (code[curline].type == ADDIU)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] + registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] + d2);
		}
		if (code[curline].type == SUB)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] - registers[d2];
			if (op == 0) registers[d] = registers[d1] - d2;
		}
		if (code[curline].type == SUBU)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] - registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] - d2);
		}
		if (code[curline].type == MUL)
		{
			if (code[curline].cont.size() == 4)
			{
				int op = code[curline].cont[0];
				int d = code[curline].cont[1];
				int d1 = code[curline].cont[2];
				int d2 = code[curline].cont[3];
				if (op == 1) registers[d] = registers[d1] * registers[d2];
				if (op == 0) registers[d] = registers[d1] * d2;
			}
			else
			{
				int op = code[curline].cont[0];
				int d1 = code[curline].cont[1];
				int d2 = code[curline].cont[2];
				if (op == 1)
				{
					registers[33] = (registers[d1] * registers[d2]) % 4294967296;
					registers[32] = (registers[d1] * registers[d2]) / 4294967296;
				}
				if (op == 0)
				{
					registers[33] = (registers[d1] * d2) % 4294967296;
					registers[32] = (registers[d1] * d2) / 4294967296;
				}
			}
		}
		if (code[curline].type == MULU)
		{
			if (code[curline].cont.size() == 4)
			{
				int op = code[curline].cont[0];
				int d = code[curline].cont[1];
				int d1 = code[curline].cont[2];
				int d2 = code[curline].cont[3];
				if (op == 1) registers[d] = (unsigned int)(registers[d1] * registers[d2]);
				if (op == 0) registers[d] = (unsigned int)(registers[d1] * d2);
			}
			else
			{
				int op = code[curline].cont[0];
				int d1 = code[curline].cont[1];
				int d2 = code[curline].cont[2];
				if (op == 1)
				{
					registers[33] = (registers[d1] * registers[d2]) % 4294967296;
					registers[32] = (registers[d1] * registers[d2]) / 4294967296;
				}
				if (op == 0)
				{
					registers[33] = (registers[d1] * d2) % 4294967296;
					registers[32] = (registers[d1] * d2) / 4294967296;
				}
			}
		}
		if (code[curline].type == DIV)
		{
			if (code[curline].cont.size() == 4)
			{
				int op = code[curline].cont[0];
				int d = code[curline].cont[1];
				int d1 = code[curline].cont[2];
				int d2 = code[curline].cont[3];
				if (op == 1) registers[d] = registers[d1] / registers[d2];
				if (op == 0) registers[d] = registers[d1] / d2;
			}
			else
			{
				int op = code[curline].cont[0];
				int d1 = code[curline].cont[1];
				int d2 = code[curline].cont[2];
				if (op == 1)
				{
					registers[33] = (registers[d1] / registers[d2]);
					registers[32] = (registers[d1] % registers[d2]);
				}
				if (op == 0)
				{
					registers[33] = (registers[d1] / d2);
					registers[32] = (registers[d1] % d2);
				}
			}
		}
		if (code[curline].type == DIVU)
		{
			if (code[curline].cont.size() == 4)
			{
				int op = code[curline].cont[0];
				int d = code[curline].cont[1];
				int d1 = code[curline].cont[2];
				int d2 = code[curline].cont[3];
				if (op == 1) registers[d] = (unsigned int)(registers[d1] / registers[d2]);
				if (op == 0) registers[d] = (unsigned int)(registers[d1] / d2);
			}
			else
			{
				int op = code[curline].cont[0];
				int d1 = code[curline].cont[1];
				int d2 = code[curline].cont[2];
				if (op == 1)
				{
					registers[33] = (unsigned int)(registers[d1] % registers[d2]);
					registers[32] = (unsigned int)(registers[d1] / registers[d2]);
				}
				if (op == 0)
				{
					registers[33] = (unsigned int)(registers[d1] % d2);
					registers[32] = (unsigned int)(registers[d1] / d2);
				}
			}
		}
		if (code[curline].type == XOR)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] ^ registers[d2];
			if (op == 0) registers[d] = registers[d1] ^ d2;
		}
		if (code[curline].type == XORU)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] ^ registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] ^  d2);
		}
		if (code[curline].type == NEG)
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = -registers[d1];
		}
		if (code[curline].type == NEGU)
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = (unsigned int)(~registers[d1]);
		}
		if (code[curline].type == REM)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] % registers[d2];
			if (op == 0) registers[d] = registers[d1] % d2;
		}
		if (code[curline].type == REMU)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] % registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] % d2);
		}
		if (code[curline].type == LI)
		{
			int d = code[curline].cont[0];
			int imm = code[curline].cont[1];
			registers[d] = imm;
		}
		if (code[curline].type == SEQ)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] == registers[d2]);
			if (op == 0) registers[d] = (registers[d1] == d2);
		}
		if (code[curline].type == SGE)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] >= registers[d2]);
			if (op == 0) registers[d] = (registers[d1] >= d2);
		}
		if (code[curline].type == SGT)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] > registers[d2]);
			if (op == 0) registers[d] = (registers[d1] > d2);
		}
		if (code[curline].type == SLE)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] <= registers[d2]);
			if (op == 0) registers[d] = (registers[d1] <= d2);
		}
		if (code[curline].type == SLT)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] < registers[d2]);
			if (op == 0) registers[d] = (registers[d1] < d2);
		}
		if (code[curline].type == SNE)
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] != registers[d2]);
			if (op == 0) registers[d] = (registers[d1] != d2);
		}
		if (code[curline].type == LA)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			registers[d] = add;
		}
		if (code[curline].type == LB)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			registers[d] = 0;
			memcpy(&registers[d], &memory[add], 1);
		}
		if (code[curline].type == LH)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			registers[d] = 0;
			memcpy(&registers[d], &memory[add], 2);
		}
		if (code[curline].type == LW)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			registers[d] = 0;
			memcpy(&registers[d], &memory[add], 4);
		}
		if (code[curline].type == SB)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			memcpy(&memory[add], &registers[d], 1);
		}
		if (code[curline].type == SH)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			memcpy(&memory[add], &registers[d], 2);
		}
		if (code[curline].type == SW)
		{
			int d = code[curline].cont[0];
			int add;
			if (code[curline].cont.size() == 3)
			{
				int x1 = code[curline].cont[1];
				int x2 = code[curline].cont[2];
				add = registers[x2] + x1;
			}
			else
			{
				add = ptrval[code[curline].cont[1]];
			}
			memcpy(&memory[add], &registers[d], 4);
		}
		if (code[curline].type == MOVE)
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = registers[d1];
		}
		if (code[curline].type == MFHI)
		{
			int d = code[curline].cont[0];
			registers[d] = registers[32];
		}
		if (code[curline].type == MFLO)
		{
			int d = code[curline].cont[0];
			registers[d] = registers[33];
		}
		if (code[curline].type == SYSCALL)
		{
			int com = registers[2];
			int ret = syscall(com);
			if (ret == 0) break;
		}
		if (code[curline].type == B)
		{
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == BEQZ)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] == registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] == d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BNE)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] != registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] != d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BGE)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] >= registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] >= d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BLE)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] <= registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] <= d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BGT)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] > registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] > d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BLT)
		{
			int d1 = code[curline].cont[1];
			int d2 = code[curline].cont[2];
			bool f = false;
			if (code[curline].cont[0] == 1)
			{
				f = (registers[d1] < registers[d2]);
			}
			if (code[curline].cont[0] == 0)
			{
				f = (registers[d1] < d2);
			}
			if (f)
			{
				int lab = code[curline].cont[3];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BEQZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r == 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BNEZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r != 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BLEZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r <= 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BGEZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r >= 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BGTZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r > 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == BLTZ)
		{
			int d = code[curline].cont[0];
			int r = registers[d];
			if (r < 0)
			{
				int lab = code[curline].cont[1];
				int pos = labval[lab];
				curline = pos;
				continue;
			}
		}
		if (code[curline].type == J)
		{
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == JAL)
		{
			registers[31] = curline + 1;
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == JR)
		{
			int d = code[curline].cont[0];
			int pos = registers[d];
			curline = pos;
			continue;
		}
		if (code[curline].type == JALR)
		{
			registers[31] = curline + 1;
			int d = code[curline].cont[0];
			int pos = registers[d];
			curline = pos;
			continue;
		}
		//cout << "command: " << code[curline].type << "\t\t\t\t";
		//cout << "curline: " << curline << '\n';
		//if (curline == 679)
		//cout << '?';
		curline++;
	}
	fs.close();
	//system("PAUSE");
	//getchar();
	return 0;
}
