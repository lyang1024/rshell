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
    while (true) {
        line.clear();
        prompt();                // Print prompt
        getline(cin,input);         // Read command line
        char *tmps;
        tmps = new char[sizeof(input)]; 
        strcpy(tmps, input.c_str()); 
        line.setCmd(tmps);          // Put command line into line
        line.parse();            // Parse the command line
        line.execute();       // Execute the command line with connectors
        
    }
    return 0;
}
