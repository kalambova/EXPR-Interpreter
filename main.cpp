#include <iostream>
#include<fstream>
#include "interpreter.h"
using namespace std;

int main()
{


    ifstream input;
    string line;
    Interpreter i;
    input.open("example.txt", ios::in);
    if(input.is_open())
    {
        while(getline(input,line))
        {
            i.execute(line);
        }
        input.close();
    }

    return 0;
}