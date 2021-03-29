//
// Created by akim on 01.03.2021.
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char* stack;
int Top = 0;

void push(char k) {
    stack[Top++] = k;
}
char pop() {
    if(!Top)
        return '0';
    char k = stack[Top - 1];
    Top--;
    return k;
}
int is_right(string s) {
    char s1;
    int k = 0;
    while(k != s.length()) {
        if((s[k] == '(' || s[k] == '[' || s[k] == '{'))
            push(s[k]);
        else if((s[k] == ')' || s[k] == ']' || s[k] == '}')) {
            if(!Top)
                return k;
            s1 = pop();
            if((s1 == '(' && s[k] != ')') || (s1 == '{' && s[k] != '}') || (s1 == '[' && s[k] != ']'))
                return k;
        }
        k++;
    }
    if(Top)
        return s.length();
    return -1;
}
int main()
{
    ifstream fin("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/input.txt");
    ofstream fout("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/output.txt");
    stack = new char[1000002];
    string s;
    fin >> s;
    int result = is_right(s);
    if(result != -1)
        fout << "NO\n" << result;
    else
        fout << "YES";
    Top = 0;

    fin.close();
    fout.close();
    delete[] stack;
    return 0;
}
