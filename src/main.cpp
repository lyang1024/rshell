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
    while (true) {
        MultiCom line;
        string input;
        int eflag;
        prompt();                // Print prompt
        getline(cin,input);         // Read command line
        if (input == "") continue;
//        char *tmps;
//        tmps = new char[sizeof(input)]; 
//        strcpy(tmps, input.c_str()); 
        line.setCmd(input);          // Put command line into line
        line.parse();            // Parse the command line
        eflag = line.execute();       // Execute the command line with connectors
        if (eflag == -1) break;
//        line.clear();
    }
    return 0;
}
