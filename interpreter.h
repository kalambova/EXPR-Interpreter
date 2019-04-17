//
// Created by eleonora on 19.01.19.
//

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <iostream>
#include <cstring>
#include <stack>
#include "variable.h"
#include "function.h"


class Interpreter {
    Variable vars;
    Function funcs;
    bool isValidFuncName(const char * funcName) const;
    bool isVar(const char * varName) const;
    int parseAsInt(const char c);
    bool isDigit(char c);
    bool isOnlyDigits(const char* str);
    unsigned long convertToInt(const char* str);
    bool hasAssignOprForFunc(const std::string line) const;
    bool hasAssignOprForVar(const std::string line) const;
    bool hasRead(const std::string line) const;
    bool hasPrint(const std::string line) const;
    bool isExpression(std::string expr);
    void assignVar(const std::string varName, std::string expr);
    void assignFunc(const std::string fx,const std::string expr);
    void read( std::string expr);
    void print(std::string expr);
    void processOperation(std::stack<int> &vStack, std::stack<char> &opStack);
    void processClosingParenthesis(stack<int> &vStack, stack<char> &opStack);
    bool OperatorCausesEvaluation(char op, char prevOp);
    void ProcessInputOperator(char op, stack<int> &vStack, stack<char>& opStack);
    int ProcessInputNumber(const char* exp, int pos, stack<int> &vStack, unsigned int lineSize);
    int ProcessVariable(const char* exp, int pos, stack<int> &vStack, unsigned int lineSize);
    unsigned long int calculateExpr(string exp, size_t lineSize);
public:
    void execute(std::string line);



};


#endif //INTERPRETER_INTERPRETER_H
