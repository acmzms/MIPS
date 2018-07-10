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
    ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, S0 ,
    S1, S2, S3, S4, S5, S6, S7, T8, T9, GP, SP, FP, RA, HI, LO, PC
};
vector<line> code;
char memory[4194312] = {0};
int registers[36] = {0};
map<string, int> labels;
map<string, int> pointers;
int heapptr = 0;
int state = 0;
int main()
{
    ifstream fs("test2.txt");
	//cout << boolalpha << fs.rdbuf()->is_open() << endl;
	//int ttt;
	//cin >> ttt;
	//if (!fs) throw;
    line xx;
    code.push_back(xx);
	registers[29] = 4194304;
    int curline = 1, runpos, y;
    while(1)
    {
        y = readline(fs);
        if(y == 0) break;
        if(code[curline].type != "main:" && code[curline].type != "" && code[curline].type[code[curline].type.size() - 1] == ':')
        {
            //labels.insert(code[curline].type, curline);
			string ss = (code[curline]).type;
			ss.pop_back();
            if(state == 1) labels[ss] = curline;
            if(state == 0) pointers[ss] = heapptr;
        }
        if(code[curline].type == "main:")
        {
            //labels.insert(code[curline].type, curline);
			string ss = (code[curline]).type;
			ss.pop_back();
			if (state == 1) labels[ss] = curline;
			if (state == 0) pointers[ss] = heapptr;
            runpos = curline;
        }
        if(code[curline].type == ".byte" && state == 0)
        {
            for(size_t i = 0;i < code[curline].cont.size();i++)
            {
                registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 1);
				heapptr++;
            }
        }
        if(code[curline].type == ".half" && state == 0)
        {
            for(size_t i = 0;i < code[curline].cont.size();i++)
            {
				registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 2);
				heapptr += 2;
            }
        }
        if(code[curline].type == ".word" && state == 0)
        {
            for(size_t i = 0;i < code[curline].cont.size();i++)
            {
                registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 4);
				heapptr += 4;
            }
        }
        if(code[curline].type == ".ascii" && state == 0)
        {
            ascii(code[curline].cont[0]);
        }
        if(code[curline].type == ".asciiz" && state == 0)
        {
            asciiz(code[curline].cont[0]);
        }
        if(code[curline].type == ".align" && state == 0)
        {
            align(stringtot<int>(code[curline].cont[0]));
        }
        if(code[curline].type == ".space" && state == 0)
        {
            space(stringtot<int>(code[curline].cont[0]));
        }
        if(code[curline].type == ".data") state = 0;
        if(code[curline].type == ".text") state = 1;
        curline++;
    }
	int endline = curline;
    curline = runpos;
    while(curline < endline)
    {
        if(code[curline].type == ".byte")
        {
			for (size_t i = 0; i < code[curline].cont.size(); i++)
			{
				registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 1);
				heapptr++;
			}
        }
        if(code[curline].type == ".half")
        {
			for (size_t i = 0; i < code[curline].cont.size(); i++)
			{
				registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 2);
				heapptr += 2;
			}
        }
        if(code[curline].type == ".word")
        {
			for (size_t i = 0; i < code[curline].cont.size(); i++)
			{
				registers[35] = stringtot<int>(code[curline].cont[i]);
				memcpy(&memory[heapptr], &registers[35], 4);
				heapptr += 4;
			}
        }
        if(code[curline].type == ".ascii")
        {
            ascii(code[curline].cont[0]);
        }
        if(code[curline].type == ".asciiz")
        {
            asciiz(code[curline].cont[0]);
        }
        if(code[curline].type == ".align")
        {
            align(stringtot<int>(code[curline].cont[0]));
        }
        if(code[curline].type == ".space")
        {
            space(stringtot<int>(code[curline].cont[0]));
        }
        if(code[curline].type == "add")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = registers[d1] + registers[d2];
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = registers[d1] + imm;
            }
        }
        if(code[curline].type == "addu")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] + registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] + imm);
            }
        }
        if(code[curline].type == "addiu")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            int imm = stringtot<int>(code[curline].cont[2]);
            registers[d] = (unsigned int)(registers[d1] + imm);
        }
        if(code[curline].type == "sub")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = registers[d1] - registers[d2];
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = registers[d1] - imm;
            }
        }
        if(code[curline].type == "subu")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] - registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] - imm);
            }
        }
        if(code[curline].type == "mul")
        {
            if(code[curline].cont.size() == 3)
            {
                int d = registoint(code[curline].cont[0]);
                int d1 = registoint(code[curline].cont[1]);
                if(code[curline].cont[2][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[2]);
                    registers[d] = registers[d1] * registers[d2];
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[2]);
                    registers[d] = registers[d1] * imm;
                }
            }
            else
            {
                int d1 = registoint(code[curline].cont[0]);
                if(code[curline].cont[1][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[1]);
                    registers[33] = (registers[d1] * registers[d2]) % 4294967296;
                    registers[32] = (registers[d1] * registers[d2]) / 4294967296;
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[1]);
                    registers[33] = (registers[d1] * imm) % 4294967296;
                    registers[32] = (registers[d1] * imm) / 4294967296;
                }
            }
        }
        if(code[curline].type == "mulu")
        {
            if(code[curline].cont.size() == 3)
            {
                int d = registoint(code[curline].cont[0]);
                int d1 = registoint(code[curline].cont[1]);
                if(code[curline].cont[2][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[2]);
                    registers[d] = (unsigned int)(registers[d1] * registers[d2]);
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[2]);
                    registers[d] = (unsigned int)(registers[d1] * imm);
                }
            }
            else
            {
                int d1 = registoint(code[curline].cont[0]);
                if(code[curline].cont[1][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[1]);
                    registers[33] = (unsigned int)((registers[d1] * registers[d2]) % 4294967296);
                    registers[32] = (unsigned int)((registers[d1] * registers[d2]) / 4294967296);
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[1]);
                    registers[33] = (unsigned int)((registers[d1] * imm) % 4294967296);
                    registers[32] = (unsigned int)((registers[d1] * imm) / 4294967296);
                }
            }
        }
        if(code[curline].type == "div")
        {
            if(code[curline].cont.size() == 3)
            {
                int d = registoint(code[curline].cont[0]);
                int d1 = registoint(code[curline].cont[1]);
                if(code[curline].cont[2][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[2]);
                    registers[d] = registers[d1] / registers[d2];
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[2]);
                    registers[d] = registers[d1] / imm;
                }
            }
            else
            {
                int d1 = registoint(code[curline].cont[0]);
                if(code[curline].cont[1][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[1]);
                    registers[33] = (registers[d1] / registers[d2]);
                    registers[32] = (registers[d1] % registers[d2]);
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[1]);
                    registers[33] = (registers[d1] / imm);
                    registers[32] = (registers[d1] % imm);
                }
            }
        }
        if(code[curline].type == "divu")
        {
            if(code[curline].cont.size() == 3)
            {
                int d = registoint(code[curline].cont[0]);
                int d1 = registoint(code[curline].cont[1]);
                if(code[curline].cont[2][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[2]);
                    registers[d] = (unsigned int)(registers[d1] / registers[d2]);
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[2]);
                    registers[d] = (unsigned int)(registers[d1] / imm);
                }
            }
            else
            {
                int d1 = registoint(code[curline].cont[0]);
                if(code[curline].cont[1][0] == '$')
                {
                    int d2 = registoint(code[curline].cont[1]);
                    registers[33] = (unsigned int)((registers[d1] / registers[d2]));
                    registers[32] = (unsigned int)((registers[d1] % registers[d2]));
                }
                else
                {
                    int imm = stringtot<int>(code[curline].cont[1]);
                    registers[33] = (unsigned int)((registers[d1] / imm));
                    registers[32] = (unsigned int)((registers[d1] % imm));
                }
            }
        }
        if(code[curline].type == "xor")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = registers[d1] ^ registers[d2];
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = registers[d1] ^ imm;
            }
        }
        if(code[curline].type == "xoru")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] ^ registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] ^ imm);
            }
        }
        if(code[curline].type == "neg")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            registers[d] = -registers[d1];
        }
        if(code[curline].type == "negu")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            registers[d] = (unsigned int)(~registers[d1]);
        }
        if(code[curline].type == "rem")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = registers[d1] % registers[d2];
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = registers[d1] % imm;
            }
        }
        if(code[curline].type == "remu")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] % registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (unsigned int)(registers[d1] % imm);
            }
        }
        if(code[curline].type == "li")
        {
            int d = registoint(code[curline].cont[0]);
            int imm = stringtot<int>(code[curline].cont[1]);
            registers[d] = imm;
        }
        if(code[curline].type == "seq")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] == registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] == imm);
            }
        }
        if(code[curline].type == "sge")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] >= registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] >= imm);
            }
        }
        if(code[curline].type == "sgt")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] > registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] > imm);
            }
        }
        if(code[curline].type == "sle")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] <= registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] <= imm);
            }
        }
        if(code[curline].type == "slt")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] < registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] < imm);
            }
        }
        if(code[curline].type == "sne")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            if(code[curline].cont[2][0] == '$')
            {
                int d2 = registoint(code[curline].cont[2]);
                registers[d] = (registers[d1] != registers[d2]);
            }
            else
            {
                int imm = stringtot<int>(code[curline].cont[2]);
                registers[d] = (registers[d1] != imm);
            }
        }
        if(code[curline].type == "la")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            registers[d] = add;
        }
        if(code[curline].type == "lb")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //registers[d] = loadb(add);
			memcpy(&registers[d], &memory[add], 1);
        }
        if(code[curline].type == "lh")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //registers[d] = loadh(add);
			memcpy(&registers[d], &memory[add], 2);
        }
        if(code[curline].type == "lw")
        {
			//if (curline == 463)
				//cout << '?';
			int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //registers[d] = loadw(add);
			memcpy(&registers[d], &memory[add], 4);
        }
        if(code[curline].type == "sb")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //storeb(d, add);
			memcpy(&memory[add], &registers[d], 1);
        }
        if(code[curline].type == "sh")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //storeh(d, add);
			memcpy(&memory[add], &registers[d], 2);
        }
        if(code[curline].type == "sw")
        {
            int d = registoint(code[curline].cont[0]);
			string tmp = code[curline].cont[1];
			int add;
			if (tmp.back() == ')')
			{
				int dis = separate(tmp);
				int d1 = registoint(tmp);
				add = registers[d1] + dis;
			}
			else
			{
				add = pointers.find(tmp)->second;
			}
            //storew(d, add);
			memcpy(&memory[add], &registers[d], 4);
        }
        if(code[curline].type == "move")
        {
            int d = registoint(code[curline].cont[0]);
            int d1 = registoint(code[curline].cont[1]);
            registers[d] = registers[d1];
        }
        if(code[curline].type == "mfhi")
        {
            int d = registoint(code[curline].cont[0]);
            registers[d] = registers[32];
        }
        if(code[curline].type == "mflo")
        {
            int d = registoint(code[curline].cont[0]);
            registers[d] = registers[33];
        }
        if(code[curline].type == "syscall")
        {
            int com = registers[2];
			//if (com == 4)
				//cout << ' ';
            int ret = syscall(com);
            if(ret == 0) break;
        }
        if(code[curline].type == "b")
        {
            string lab = code[curline].cont[0];
            int pos = (labels.find(lab))->second;
            curline = pos;
            continue;
        }
        if(code[curline].type == "beq")
        {
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] == cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bne")
        {
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] != cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bge")
        {
			//if (curline == 366)
				//cout << '?';
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] >= cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "ble")
        {
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] <= cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bgt")
        {
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] > cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "blt")
        {
            int d = registoint(code[curline].cont[0]);
            int cmp;
            if(code[curline].cont[1][0] == '$')
            {
                int d1 = registoint(code[curline].cont[1]);
                cmp = registers[d1];
            }
            else
            {
                cmp = stringtot<int>(code[curline].cont[1]);
            }
            if(registers[d] < cmp)
            {
                string lab = code[curline].cont[2];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "beqz")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] == 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bnez")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] != 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "blez")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] <= 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bgez")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] >= 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bgtz")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] > 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "bltz")
        {
            int d = registoint(code[curline].cont[0]);
            if(registers[d] < 0)
            {
                string lab = code[curline].cont[1];
                int pos = (labels.find(lab))->second;
                curline = pos;
                continue;
            }
        }
        if(code[curline].type == "j")
        {
            string lab = code[curline].cont[0];
            int pos = (labels.find(lab))->second;
            curline = pos;
            continue;
        }
        if(code[curline].type == "jal")
        {
			registers[31] = curline + 1;
            string lab = code.at(curline).cont[0];
            int pos = labels.find(lab)->second;
            curline = pos;
            continue;
        }
        if(code[curline].type == "jr")
        {
			//if (curline == 466)
				//cout << '?';
            int d = registoint(code[curline].cont[0]);
            int pos = registers[d];
            curline = pos;
            continue;
        }
        if(code[curline].type == "jalr")
        {
			registers[31] = curline + 1;
            int d = registoint(code[curline].cont[0]);
            int pos = registers[d];
            curline = pos;
            continue;
        }
		//cout << "command: " << code[curline].type << "\t\t\t\t";
		//cout << "curline: " << curline << '\n';
		//if (curline == 836)
			//cout << '?';
        curline++;
    }
    fs.close();
	system("PAUSE");
    return 0;
}
