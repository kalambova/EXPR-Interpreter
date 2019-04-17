#include <iostream>
#include "interpreter.h"
#include <stdlib.h>
#include <cstring>

bool Interpreter::isValidFuncName(const char * funcName) const
{
    for(int i=0;funcName[i]!='\0';i++)
    {
        if(funcName[i]< 'A' || funcName[i] > 'Z')
        {
            //std::cout<<"Inncorect name of variable "<<varName<<std::endl;
            return false;
        }
    }
    return true;
}
bool Interpreter::isVar(const char * varName) const
{
    for(int i=0;varName[i]!='\0';i++)
    {
        if(varName[i]< 'a' || varName[i] > 'z')
        {
            //std::cout<<"Inncorect name of variable "<<varName<<std::endl;
            return false;
        }
    }
    return true;
}
int Interpreter::parseAsInt(const char c)
{
    return c-'0';
}
bool Interpreter::isDigit(char c)
{
    return c>='0'&&c<='9';
}
bool Interpreter::isOnlyDigits(const char* str)
{
    for(int i=0;str[i]!='\0';i++)
    {
        if(!isDigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

unsigned long Interpreter::convertToInt(const char* str)
{

    unsigned long result=0;
    for(int i=0;str[i]!='\0';i++)
    {
        if(str[i+1]!='\0')
        {
            result+=parseAsInt(str[i]);
            result*=10;
        }
        else  result+=parseAsInt(str[i]);

    }
    return result;


}
bool Interpreter::hasAssignOprForFunc(const std::string line) const
{
    size_t indexOfFirstBracket = line.find('[');
    size_t indexOfSecondBracket= line.find(']');
    if(indexOfFirstBracket>line.size()||indexOfSecondBracket>line.size())
    {
        return false;
    }
    std::string funcName=line.substr(0,indexOfFirstBracket);
    std::string parameter=line.substr(indexOfFirstBracket+1,indexOfSecondBracket-indexOfFirstBracket-1);
    return isValidFuncName(funcName.c_str()) && isVar(parameter.c_str())  && line[indexOfSecondBracket+2] == '=';
}
bool Interpreter::hasAssignOprForVar(const std::string line) const
{

    size_t indexOfEqualSign = line.find('=');
    if(indexOfEqualSign>line.size())
    {
        return false;
    }
    std::string varName =line.substr(0,indexOfEqualSign-1);
    return isVar(varName.c_str()) && line[indexOfEqualSign] == '=';
}
bool Interpreter::hasRead(const std::string line) const
{
    std::string varName = line.substr(5,line.size());
    return line.substr(0,5) == "read " && isVar(varName.c_str());
}
bool Interpreter::hasPrint(const std::string line) const
{
    return line.substr(0,5) == "print";
}
bool Interpreter::isExpression(std::string expr)
{
    if(expr.find('+')<expr.size()||expr.find('-')<expr.size()||expr.find('*')<expr.size()||expr.find('/')<expr.size()||expr.find('%')<expr.size())
    {
        return true;
    }
    return false;
}
void Interpreter::processOperation(std::stack<int> &vStack, std::stack<char> &opStack)
{

    int rightOperand = vStack.top();
    vStack.pop();
    int leftOperand = vStack.top();
    vStack.pop();
    char op = opStack.top();
    opStack.pop();

    int result = 0;
    switch (op)
    {
        case '+':
            result = leftOperand + rightOperand;
            break;
        case '-':
            result = leftOperand - rightOperand;
            break;
        case '*':
            result = leftOperand * rightOperand;
            break;
        case '/':
            result = leftOperand / rightOperand;
            break;
        case '%':
            result = leftOperand % rightOperand;
            break;
    }

    vStack.push(result);

}
void Interpreter::processClosingParenthesis(stack<int> &vStack, stack<char> &opStack)
{

    while (opStack.top() != '(')
        processOperation(vStack, opStack);

    opStack.pop(); // Remove the opening parenthesis

}
bool Interpreter::OperatorCausesEvaluation(char op, char prevOp)
{

    bool evaluate = false;

    switch (op)
    {
        case '+':
        case '-':
            evaluate = (prevOp != '(');
            break;
        case '*':
        case '/':
        case '%':
            evaluate = (prevOp == '*' || prevOp == '/' || prevOp == '%');
            break;
        case ')':
            evaluate = true;
            break;
    }

    return evaluate;

}
//if we still have operators in the stack and if OperatorCausesEvaluation return true -
//execute the operation and push the current operation in the stack(to have previous one)
void Interpreter::ProcessInputOperator(char op, stack<int> &vStack, stack<char>& opStack)
{

    while (opStack.size() > 0 &&
           OperatorCausesEvaluation(op, opStack.top()))
        processOperation(vStack, opStack);

    opStack.push(op);

}
//converts the string numbers in the express to int and pushes them in the stack
int Interpreter::ProcessInputNumber(const char* exp, int pos, stack<int> &vStack, unsigned int lineSize)
{
    int value = 0;
    while (pos < lineSize && (exp[pos] >= '0' && exp[pos] <= '9'))
        value = 10 * value + (int)(exp[pos++] - '0');

    vStack.push(value);

    return pos;

}
//finds the whole variable in the expression and checks if it is in the map of variable->value
//if it is there -> push to stack with values
int Interpreter::ProcessVariable(const char* exp, int pos, stack<int> &vStack, unsigned int lineSize)
{
    string varKey;
    while (pos < lineSize && (exp[pos] >= 'a' && exp[pos] <= 'z'))
        varKey += exp[pos++];

    if (vars.isMember(varKey))
    {
        unsigned int valueByKey = 0;
        valueByKey = vars.getValueOf(varKey);
        vStack.push(valueByKey);
    }
    else
    {
        return -1; //indicates an error(in this case)
    }

    return pos;
}

//main function of the method - reads the expression, makes it into operands and operators
//calls the upper functions to calculate the expression and return the result
unsigned long int Interpreter::calculateExpr(string exp, size_t lineSize)
{

    stack<int> vStack;
    stack<char> opStack;

    const char* expr = exp.c_str();

    opStack.push('('); // Implicit opening parenthesis

    int pos = 0;
    while (pos <= lineSize)
    {
        if (pos == lineSize || expr[pos] == ')')
        {
            processClosingParenthesis(vStack, opStack);
            pos++;
        }
        else if (expr[pos] >= '0' && expr[pos] <= '9')
        {
            pos = ProcessInputNumber(expr, pos, vStack, lineSize);
        }
        else if (expr[pos] >= 'a' && expr[pos] <= 'z')
        {
            pos = ProcessVariable(expr, pos, vStack, lineSize);
            if (pos == -1)
            {
                cerr << "Error: Missing variable declaration" << endl;
            }
        }
        else
        {
            ProcessInputOperator(expr[pos], vStack, opStack);
            pos++;
        }

    }

    int returnVal = vStack.top();
    vStack.pop();

    return returnVal; // Result remains on values stacks

}
void Interpreter::execute(const std::string line)
{
    if(hasAssignOprForVar(line))
    {
        size_t indexOfEqualSign = line.find('=');
        std::string varName =line.substr(0,indexOfEqualSign-1);

            assignVar(varName, line.substr(indexOfEqualSign+2));


    }
    else if(hasAssignOprForFunc(line))
    {
        size_t indexOfEqualSign=line.find('=');
        std::string func=line.substr(0,indexOfEqualSign-1);
        std::string expression=line.substr(indexOfEqualSign+2);
        assignFunc(func, expression);
    }
    else if(hasRead(line))
    {
        read(line.substr(5));
    }
    else if(hasPrint(line))
    {
        print(line.substr(6));
    }
    else
        std::cerr<<"Syntax error \n";
}
void Interpreter::assignVar(const std::string varName, std::string expr)
{
    if(isOnlyDigits(expr.c_str())){
        vars.add(varName, convertToInt(expr.c_str()));
    }

    else if(isExpression(expr))
        {
        vars.add(varName,calculateExpr(expr,expr.size()));
        }
        else
            {
                std::cerr<<"Syntax error \n";
                exit(0);
            }

}
void Interpreter::assignFunc(const std::string fx,const std::string expr)
{
    funcs.add(make_pair(fx,expr));
}

void Interpreter::read(const std::string expr)
{
    unsigned long v;
    std::cout<<"read "<<expr<<" = ";
    std::cin>>v;
    vars.add(expr, v);
}
void replaceParamInExpr(string &expr,string arg,string param)
{
    for(int i=0;i<expr.size();i++)
    {
        if(expr.substr(i,arg.size()+i)==arg)
        {
            expr.replace(i,arg.size()+1,param);
        }
    }
}
void Interpreter::print(std::string expr)
{

        if(vars.isMember(expr))
        {

            std::cout<<vars.getValueOf(expr)<<std::endl;
        }

        else if(isValidFuncName(expr.substr(0,expr.find('[')).c_str()))
        {
            size_t indexOfFirstBracket = expr.find('[');
            size_t indexOfSecondBracket= expr.find(']');
            std::string param =expr.substr(indexOfFirstBracket+1,indexOfSecondBracket-indexOfFirstBracket-1);
            if(!vars.isMember(param))
            {
                if(isOnlyDigits(param.c_str()))
                {
                    std::string funcName=expr.substr(0,indexOfFirstBracket);
                    string expression=funcs.getExprByFname(funcName);
                    string argument = funcs.getFArgByFname(funcName);
                    replaceParamInExpr(expression,argument,param);
                    cout<<calculateExpr(expression,expression.size())<<endl;
                }else
                {
                    cerr<<"No variable "<<param<<"\n";
                }

            }
            else
            {
                std::string funcName=expr.substr(0,indexOfFirstBracket);
                string expression=funcs.getExprByFname(funcName);
                string argument = funcs.getFArgByFname(funcName);
                replaceParamInExpr(expression,argument,param);
                cout<<calculateExpr(expression,expression.size())<<endl;
            }
        }
    else
        cout<<calculateExpr(expr,expr.size())<<endl;

}
