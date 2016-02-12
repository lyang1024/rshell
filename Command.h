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

/* Base class */
class Command {
    
public:
    virtual void parse() = 0;
    virtual int execute() = 0;
    virtual void clear() = 0;
    virtual void setCmd(char *s) = 0;
};

/* Single command class*/
class SingleCom: public Command {
protected:
    char cmd[100];
    char *tokens[64];
    int flag;
    
public:
    
    void clear() {
        memset(tokens, 0, sizeof(tokens));
        memset(cmd, 0, sizeof(cmd));
    }
   /* put the single command into SingleCom class*/
    void setCmd(char *s) {
        strcpy(cmd, s);
    }
    
    //parse single command into tokens
    void parse() {
        int count = 0;
        char *token = strtok(cmd, " ");
        while (token != NULL) {
            tokens[count] = new char[strlen(token)];
            strcpy(tokens[count], token);
            token = strtok(NULL, " ");
            count++;
        }
        tokens[count] = 0;
        delete[] token;
        token = NULL;
        return;
    }
    //execute the single command
    int execute() {
        pid_t pid;
        flag = 1;      // identify that the command execute successfully(=1) or not(=0)
        int status;

	// If single command is "exit" then exit 
        if ((strcmp(tokens[0], "exit") == 0) && (tokens[1] == 0)) exit(0);
        
	// Execute the single command
	if ((pid = fork()) < 0) {
            perror("ERROR: Forking child process failure\n");
            return 0;
        }
        else if (pid == 0) {
            execvp(tokens[0], tokens);
            perror("ERROR:Execution failure\n");
            flag = 0;
            return 0;
        }
        else {
            while (wait(&status) != pid);
        }
        return flag;
    }
};

/* Multi-command class with connectors */
class MultiCom: public Command {
private:
    vector<SingleCom> coms;   // Store every single command tokenized by "||", "&&", ";"
    vector<string> connectors; // Store every connector corresponding to each single command 
    char cmdl[1000];   // Store the multi-command line;
    
public:
    
    void clear() {
        memset(cmdl, 0, sizeof(cmdl));
        coms.clear();
        connectors.clear();
    }

    void setCmd(char *s) {
        strcpy(cmdl, s);
    }
// Tokenize the multi-command line into single commmands by connectors "||", "&&" and ";"
    void parse() {
        // Delete the comment from command line 
        char* tmp;
        tmp = strchr(cmdl, '#');
        char *cmdll;
        if (tmp != NULL) {
            cmdll = new char[strlen(cmdl) - strlen(tmp)];
            strncpy(cmdll, cmdl, strlen(cmdl) - strlen(tmp));
        }
        else {
            cmdll = new char[strlen(cmdl)];
            strcpy(cmdll, cmdl);
        }
	// Tokenize the command line into single commands sequentially
        char *tcom;
        char* tcmdl;
        tcmdl = new char[strlen(cmdll)];
        strcpy(tcmdl, cmdll);
        tcom = strtok(tcmdl, "|&;");
        while (tcom != NULL) {
            SingleCom sc;
            sc.clear();
            sc.setCmd(tcom);
            coms.push_back(sc);
            tcom = strtok(NULL, "|&;");
        }
        // Parse each single command
        for (unsigned int i=0; i<coms.size(); i++) {
            coms[i].parse();
        }
        
        //Find all the  connectors and store in the connectors vector sequentially
        string scmd(cmdll);
        string::iterator it;
        for (it = scmd.begin(); it != scmd.end(); it++) {
           // Identify connector "||" 
	   if (*it == '|') {
                ++it;
                if(*it == '|') {
                    string t = "||";
                    connectors.push_back(t);
                }
                else {
                    printf("Wrong input format!\n");
                    return;
                }
            }
	   // Identify connector "&&"
            else if (*it == '&') {
                ++it;
                if(*it =='&') {
                    string t = "&&";
                    connectors.push_back(t);
                    
                }
                else {
                    printf("Wrong input format!\n");
                    return;
                }
            }
            // Identify connector ";"
            else if (*it == ';') {
                string t = ";";
                connectors.push_back(t);
            }
        }
    }
    /* Execute the command according to the connectors. 
  Each single command follows a connector except the first one.*/
    int execute() {
        int result = 1;    // Store the status of the execution of a single command  0 = fail, 1 = succeed 
        result = coms[0].execute();
        unsigned int i = 0;
        while (i < connectors.size()) {
            if (connectors[i] == "||" && result == 1) {
                i++;
            }
            else if (connectors[i] == "&&" && result == 0) {
                i++;
            }
            else {
                i++;
                result = 1;
                result = coms[i].execute();
            }
        }
        return 1;
    }
};


#endif
