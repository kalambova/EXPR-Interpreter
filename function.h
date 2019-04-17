//
// Created by eleonora on 21.01.19.
//

#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H

#include<iostream>
#include<unordered_map>
#include<string>
using namespace std;

class Function
{
    unordered_map<string,string> table;

public:

    bool isMember(const string s);
    void add(pair<string, string>);
    string getExprByFname(const string str);
    string getFArgByFname(const string str);


};

#endif // _FUNCTIONS_H_