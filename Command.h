//
//  Command.h
//  sshell
//
//  Created by 0.0 on 2/9/16.
//  Copyright (c) 2016 0.0. All rights reserved.
//

#ifndef sshell_Command_h
#define sshell_Command_h
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<vector>
#include<string>
#include<stdlib.h>
using namespace std;

class Command{
    
public:
    virtual void parse() = 0;
    virtual int execute() = 0;
    virtual void clear() = 0;
    virtual void setCmd(char *s) = 0;
};

class SingleCom: public Command{
protected:
    char cmd[100];
    char *tokens[64];
    int flag;
    
public:
    
    void clear(){
        memset(tokens,0,sizeof(tokens));
        memset(cmd,0,sizeof(cmd));
    }
    void setCmd(char *s){
        strcpy(cmd, s);
    }
    
    //parse command into tokens
    void parse(){
        int count=0;
        char *token = strtok(cmd," ");
        while (token != NULL)
        {
            tokens[count]=new char[strlen(token)];// Some memory problem...
            strcpy(tokens[count],token);
            token = strtok(NULL," ");
            count++;
        }
        tokens[count] =0;
        delete[] token;
        token=NULL;
        return;
    }
    //execute this command
    int execute()
    {
        pid_t pid;
        flag=1;
        int status;
        if((strcmp(tokens[0],"exit") == 0)&&(tokens[1] == 0)) exit(0);
        if((pid = fork()) < 0){
            perror("ERROR: Forking child process failure\n");
            return 0;
            //exit(1);
        }
        else if (pid == 0){
            // Need to use pipe() to transfer the value of flag from child to parent
            execvp(tokens[0],tokens);
            perror("ERROR:Execution failure\n");
            flag=0;
            return 0;
            //exit(0);
        }
        else{
            while (wait(&status) != pid);
        }
        return flag;
    }
};


class MultiCom: public Command{
private:
    vector<SingleCom> coms;
    vector<string> connectors;
    char cmdl[1000];
    
public:
    
    void clear(){
        memset(cmdl, 0, sizeof(cmdl));
        coms.clear();
        connectors.clear();
    }
    void setCmd(char *s){
        strcpy(cmdl, s);
    }
    void parse(){
        
        char* tmp;
        tmp = strchr(cmdl,'#');
        char *cmdll;
        if (tmp != NULL){
            cmdll=new char[strlen(cmdl)-strlen(tmp)];
            strncpy(cmdll,cmdl,strlen(cmdl)-strlen(tmp));
        }
        else{
            cmdll=new char[strlen(cmdl)];
            strcpy(cmdll,cmdl);
        }
        char *tcom;
        char* tcmdl;
        tcmdl=new char[strlen(cmdll)];
        strcpy(tcmdl, cmdll);
        tcom = strtok(tcmdl,"|&;");
        while (tcom != NULL){
            SingleCom sc;
            sc.clear();
            sc.setCmd(tcom);
            coms.push_back(sc);
            tcom = strtok(NULL,"|&;");
        }
        for (unsigned int i=0; i<coms.size();i++)
        {
            coms[i].parse();
        }
        
        //find connectors
        string scmd(cmdll);
        string::iterator it;
        for (it = scmd.begin();it != scmd.end();it++){
            if (*it == '|'){
                ++it;
                if(*it == '|'){
                    string t = "||";
                    connectors.push_back(t);
                }
                else{
                    printf("Wrong input format!\n");
                    return;
                }
            }
            else if (*it == '&'){
                ++it;
                if(*it =='&'){
                    string t = "&&";
                    connectors.push_back(t);
                    
                }
                else{
                    printf("Wrong input format!\n");
                    return;
                }
            }
            else if (*it == ';'){
                string t = ";";
                connectors.push_back(t);
            }
        }
    }
    
    int execute()
    {
        int result=1;
        result = coms[0].execute();
        unsigned int i=0;
        while(i < connectors.size()){
            if (connectors[i] == "||" && result == 1){
                i++;
            }
            else if (connectors[i] == "&&" && result == 0){
                i++;
            }
            else{
                i++;
                result=1;
                result=coms[i].execute();
            }
        }
        return 1;
    }
};


#endif
