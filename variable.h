#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include<iostream>
#include<unordered_map>
class Variable
{
    std::unordered_map<std::string, unsigned long int> table;
public:
    bool isMember(const std::string c) const;
    void add(std::string, unsigned long int);
    unsigned long int getValueOf(const std::string c);
    ;
};
#endif // _VARIABLES_H_