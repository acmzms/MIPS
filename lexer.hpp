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


void space(int n)
{
    heapptr += n;
}

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
