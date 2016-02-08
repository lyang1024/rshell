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

    protected:
    char cmd[1000];

    public:
    virtual void parse() = 0;
    virtual int execute() = 0;
    virtual void clear() = 0;
    void setCmd(string s){
        strncpy(cmd, s.c_str(), sizeof(cmd));
        cmd[sizeof(cmd)-1] = 0;
        printf("command class created 22");
    }
};

class SingleCom: public Command{
    private:
    char *tokens[64];

    public:

    void clear(){
        memset(tokens,0,sizeof(tokens));
        memset(cmd,0,sizeof(cmd));
    }

    //parse command into tokens
    void parse(){
        if (strcmp(cmd,"exit") == 0) exit(0);
        printf("parse single command 35");
        *tokens = strtok(cmd," ");
        while (tokens != NULL)
        {
            ++*tokens;
            *tokens = strtok(NULL," ");
        }
        return;
    }

    //execute this command
    int execute()
    {
        printf("execute single command 48");
        pid_t pid;
        int status;
        if((pid = fork()) < 0){
            perror("ERROR: Forking child process failure\n");
            return 0;
            //exit(1);
        }
        else if (pid == 0){
            if (execvp(*tokens,tokens) < 0){
                perror("ERROR:Execution failure\n");
                return 0;
                //exit(1);
            }
        }
        else{
            while (waitpid(-1,&status,0)!=pid){
                if (waitpid(-1,&status,0)==-1){
                    perror("ERROR: Child process failure\n");
                    return 0;
                }
            }
        }
        return 1;
    }
};


class MultiCom: public Command{
    private:
    vector<SingleCom> coms;
    vector<string> connectors;

    public:

    void clear(){
        coms.clear();
        connectors.clear();
    }

    void parse(){
        printf("parse multicommand 83");
        if (strcmp(cmd,"exit") == 0) exit(0);
        char* tmp;
        tmp = strchr(cmd,'#');
        if (tmp != NULL)
            strncpy(cmd,cmd,tmp-cmd);
        *tmp = 0;

        char* tcom;
        tcom = strtok(cmd,"|&;");
        while (tcom != NULL){
            SingleCom sc;
            sc.setCmd(tcom);
            sc.parse();
            coms.push_back(sc);
            tcom = strtok(NULL,"|&;");
        }
        *tcom = 0;

        //remove spaces
        //create a duplicate of cmd?
        //char* c1 = cmd;
        //char* c2 = cmd;
        //while (*c1){
        //    if (*c1 != ' ')
        //        *c2++ = *c1;
        //    c1++;
        //}
        //*c2 = 0;

        //find connectors
        printf("finding connectors");
        string scmd(cmd);
        string::iterator it;
        for (it = scmd.begin();it != scmd.end();it++){
            if (*it == '|'){
                ++it;
                if(*it == '|'){
                    string t = "||";
                    connectors.push_back(t);
                }
                else{
                    printf("Wrong input format!");
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
                    printf("Wrong input format!");
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
        printf("executing muticommand 149");
        int result;
        for(unsigned int i = 0;i < connectors.size();++i){
            result = coms[i].execute();
            if (connectors[i] == ";");
            else if (connectors[i] == "||" && result == 1)
                return 1;
            else if (connectors[i] == "&&" && result == 0)
                return 1;
        }
        return 1;
    }
};


