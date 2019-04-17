#include<iostream>
#include "function.h"
#include<string>
using namespace std;

bool Function::isMember(const string s)
{
    for(const pair<string, string> p:table)
    {
        if(s.compare(p.first))
            return true;
    }
    return false;
}

void Function::add(pair<string, string> p)
{
    if(table.find(p.first) == table.end())
    {
        table[p.first] = p.second;
    }
    else
        cerr<<"Trying to add variable in variables table which already exists! ";
}

string Function::getExprByFname(const string str)
{
    for(const pair<string, string> p:table)
    {
        int idx = p.first.find('[',0);
        string searchBy = (p.first).substr(0,idx);
        if(str.compare(searchBy) == 0)
            return p.second;
    }
    cerr<<"Trying to get ExprTree of non-existing element!";
}

string  Function::getFArgByFname(const string str)
{
    for(const pair<string, string> p:table)
    {
        size_t indexOfFirstBracket = p.first.find('[');
        size_t indexOfSecondBracket= p.first.find(']');
        string searchBy = (p.first).substr(0,indexOfFirstBracket);
        if(str.compare(searchBy) == 0)
            return (p.first).substr(indexOfFirstBracket+1,indexOfSecondBracket-indexOfFirstBracket-1);
    }
    cerr<<"Trying to get Argument of undefined function!";
}