#include "variable.h"
#include<iostream>
using namespace std;
bool Variable::isMember(const std::string  c) const
{
    for(auto p : table)
    {
        if(p.first == c)
            return true;
    }
    return false;
}

void Variable::add(std::string var, unsigned long int value)
{
    table[var] = value;
}
unsigned long int Variable::getValueOf(const std::string c)
{
    if(isMember(c))
        return table[c];
}

