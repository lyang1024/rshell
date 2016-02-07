#include<iostream>
using namespace std;

class Command{
    protected:
        char cmd[1000];
        char *tokens[10];
    public:
        virtual void parse();
        virtual void execute();
}

class SingleCom:public Command{
    public:
    void parse(){
        if(strcmp(cmd,"exit")==0) exit(0);
        tokens=strtok(cmd," ");
        while(tokens!=NULL)
        {
            ++tokens;
            tokens=strtok(NULL," ");
        }
        return 0;
    }
    void execute()
    {
        pid_t pid;
        int status;
        if((pid=fork())<0){
            printf("ERROR: Forking child process failure\n");
            exit(1);
        }
        else if(pid==0){
            if(execvp(*tokens,token)<0){
                printf("ERROR:Execution failure\n");
                exit(1);
            }
        }
        else{
            while


class MultiCom{
    private:
        vector<
