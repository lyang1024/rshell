
#include "Command.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

void prompt()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
        cout<<getlogin()<<"@"<<hostname<<"$ ";
    else
        cout<<getlogin()<<"@UnknownHostName$ ";
}

int main()
{
    MultiCom line;
    string input;
    while(true){
        line.clear();
        prompt();
        getline(cin,input);
//        if (input=="exit")
//            break;
        char *tmps;
        tmps=new char[sizeof(input)];
        strcpy(tmps, input.c_str());
        
        line.setCmd(tmps);
        line.parse();
        line.execute();
        
    }
    return 0;
}
