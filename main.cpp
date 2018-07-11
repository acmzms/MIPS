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
	ifstream fs("test2.txt");
	while (1)
	{
		y = readline(fs, state);
		if (y == 0) break;
		if (code[curline].type == "main")
		{
			runpos = curline;
		}
		if (code[curline].type == ".data") state = 0;
		if (code[curline].type == ".text") state = 1;
		//cout << "command: " << code[curline].type << "\t\t\t\t";
		//cout << "curline: " << curline << '\n';
		curline++;
	}
	int endline = curline;
	curline = runpos;
	while (curline < endline)
	{
		if (code[curline].type == "add")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] + registers[d2];
			if (op == 0) registers[d] = registers[d1] + d2;
		}
		if (code[curline].type == "addu")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] + registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] + d2);
		}
		if (code[curline].type == "addiu")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] + registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] + d2);
		}
		if (code[curline].type == "sub")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] - registers[d2];
			if (op == 0) registers[d] = registers[d1] - d2;
		}
		if (code[curline].type == "subu")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] - registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] - d2);
		}
		if (code[curline].type == "mul")
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
		if (code[curline].type == "mulu")
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
		if (code[curline].type == "div")
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
		if (code[curline].type == "divu")
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
		if (code[curline].type == "xor")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] ^ registers[d2];
			if (op == 0) registers[d] = registers[d1] ^ d2;
		}
		if (code[curline].type == "xoru")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] ^ registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] ^  d2);
		}
		if (code[curline].type == "neg")
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = -registers[d1];
		}
		if (code[curline].type == "negu")
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = (unsigned int)(~registers[d1]);
		}
		if (code[curline].type == "rem")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = registers[d1] % registers[d2];
			if (op == 0) registers[d] = registers[d1] % d2;
		}
		if (code[curline].type == "remu")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (unsigned int)(registers[d1] % registers[d2]);
			if (op == 0) registers[d] = (unsigned int)(registers[d1] % d2);
		}
		if (code[curline].type == "li")
		{
			int d = code[curline].cont[0];
			int imm = code[curline].cont[1];
			registers[d] = imm;
		}
		if (code[curline].type == "seq")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] == registers[d2]);
			if (op == 0) registers[d] = (registers[d1] == d2);
		}
		if (code[curline].type == "sge")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] >= registers[d2]);
			if (op == 0) registers[d] = (registers[d1] >= d2);
		}
		if (code[curline].type == "sgt")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] > registers[d2]);
			if (op == 0) registers[d] = (registers[d1] > d2);
		}
		if (code[curline].type == "sle")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] <= registers[d2]);
			if (op == 0) registers[d] = (registers[d1] <= d2);
		}
		if (code[curline].type == "slt")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] < registers[d2]);
			if (op == 0) registers[d] = (registers[d1] < d2);
		}
		if (code[curline].type == "sne")
		{
			int op = code[curline].cont[0];
			int d = code[curline].cont[1];
			int d1 = code[curline].cont[2];
			int d2 = code[curline].cont[3];
			if (op == 1) registers[d] = (registers[d1] != registers[d2]);
			if (op == 0) registers[d] = (registers[d1] != d2);
		}
		if (code[curline].type == "la")
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
		if (code[curline].type == "lb")
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
		if (code[curline].type == "lh")
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
		if (code[curline].type == "lw")
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
		if (code[curline].type == "sb")
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
		if (code[curline].type == "sh")
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
		if (code[curline].type == "sw")
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
		if (code[curline].type == "move")
		{
			int d = code[curline].cont[0];
			int d1 = code[curline].cont[1];
			registers[d] = registers[d1];
		}
		if (code[curline].type == "mfhi")
		{
			int d = code[curline].cont[0];
			registers[d] = registers[32];
		}
		if (code[curline].type == "mflo")
		{
			int d = code[curline].cont[0];
			registers[d] = registers[33];
		}
		if (code[curline].type == "syscall")
		{
			int com = registers[2];
			int ret = syscall(com);
			if (ret == 0) break;
		}
		if (code[curline].type == "b")
		{
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == "beq")
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
		if (code[curline].type == "bne")
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
		if (code[curline].type == "bge")
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
		if (code[curline].type == "ble")
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
		if (code[curline].type == "bgt")
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
		if (code[curline].type == "blt")
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
		if (code[curline].type == "beqz")
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
		if (code[curline].type == "bnez")
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
		if (code[curline].type == "blez")
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
		if (code[curline].type == "bgez")
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
		if (code[curline].type == "bgtz")
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
		if (code[curline].type == "bltz")
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
		if (code[curline].type == "j")
		{
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == "jal")
		{
			registers[31] = curline + 1;
			int lab = code[curline].cont[0];
			int pos = labval[lab];
			curline = pos;
			continue;
		}
		if (code[curline].type == "jr")
		{
			int d = code[curline].cont[0];
			int pos = registers[d];
			curline = pos;
			continue;
		}
		if (code[curline].type == "jalr")
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
	system("PAUSE");
	//getchar();
	return 0;
}