
#include "Command.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int main()
{
    MultiCom line;
    string input;
    while(true){
        line.clear();
        cout<<"$";
        getline(cin,input);
        if (input=="exit")
            break;
        char *tmps;
        tmps=new char[sizeof(input)];
        strcpy(tmps, input.c_str());
        
        line.setCmd(tmps);
        line.parse();
        line.execute();
        
    }
    return 0;
}
