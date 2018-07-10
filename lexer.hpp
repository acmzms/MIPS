#ifndef LEXER
#define LEXER
#include<cmath>
#include<vector>
#include<map>
#include<algorithm>
#include<string>
#include<iostream>
#include"algo.hpp"
extern int registers[36];
extern char memory[4194312];
extern int heapptr, stackptr;

void align(int n)
{
    while(heapptr % (int)pow(2, n) != 0) heapptr++;
}

void ascii(string str)
{
    string ss = process(str);
    for(size_t i = 0;i < ss.size();i++)
    {
        memory[heapptr] = ss[i];
        heapptr++;
    }
}

void asciiz(string str)
{
    string ss = process(str);
    for(size_t i = 0;i < ss.size();i++)
    {
        memory[heapptr] = ss[i];
        heapptr++;
    }
    memory[heapptr] = '\0';
    heapptr++;
}

/*void byte(vector<int> vi)
{
    for(size_t i = 0;i < vi.size();i++)
    {
        memory[heapptr] = bytetochar(vi[i]);
        heapptr++;
    }
}

void half(vector<int> vi)
{
    for(size_t i = 0;i < vi.size();i++)
    {
        unsigned char uc[2];
        halftochar(vi[i], uc);
        memory[heapptr] = uc[0];
        heapptr++;
        memory[heapptr] = uc[1];
        heapptr++;
    }
}

void word(vector<int> vi)
{
    for(size_t i = 0;i < vi.size();i++)
    {
        unsigned char uc[4];
        wordtochar(vi[i], uc);
        memory[heapptr] = uc[0];
        heapptr++;
        memory[heapptr] = uc[1];
        heapptr++;
        memory[heapptr] = uc[2];
        heapptr++;
        memory[heapptr] = uc[3];
        heapptr++;
    }
}*/

void space(int n)
{
    heapptr += n;
}

/*int loadb(int pos)
{
    return chartobyte(memory[pos]);
}

int loadh(int pos)
{
    return chartohalf(memory[pos], memory[pos + 1]);
}

int loadw(int pos)
{
    return chartoword(memory[pos], memory[pos + 1], memory[pos + 2], memory[pos + 3]);
}

void storeb(int rsrc, int pos)
{
	int x = registers[rsrc];
    memory[pos] = bytetochar(x);
}

void storeh(int rsrc, int pos)
{
    unsigned char uc[2];
	int x = registers[rsrc];
    halftochar(x, uc);
    memory[pos] = uc[0];
    memory[pos + 1] = uc[1];
}

void storew(int rsrc, int pos)
{
    unsigned char uc[4];
	int x = registers[rsrc];
    wordtochar(x, uc);
    memory[pos] = uc[0];
    memory[pos + 1] = uc[1];
    memory[pos + 2] = uc[2];
    memory[pos + 3] = uc[3];
}*/

int syscall(int type)
{
    if(type == 1) cout << registers[4];
    if(type == 4)
    {
        int i = 1;
        char c = (char)memory[registers[4]];
        while(c != '\0')
        {
            cout << c;
            c = (char)memory[registers[4] + i];
            i++;
        }
    }
    if(type == 5)
    {
        cin >> registers[2];
    }
    if(type == 8)
    {
        string s;
        cin >> s;
        int pos = registers[4];
        for(size_t i = 0;i < s.size() && i < (size_t)(registers[5] - 1);i++)
        {
            memory[pos] = s[i];
            pos++;
        }
    }
    if(type == 9)
    {
        int len = registers[4];
        registers[2] = heapptr;
        heapptr += len;
    }
    if(type == 10)
    {
        return 0;
    }
    if(type == 17)
    {
        registers[4] = 0;
        return 0;
    }
    return 1;
}
#endif
