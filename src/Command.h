#ifndef sshell_Command_h
#define sshell_Command_h
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<iostream>
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
    int count;
    
public:
    void clear() {
        memset(tokens, 0, sizeof(tokens));
        memset(cmd, 0, sizeof(cmd));
    }
   /* put the single command into SingleCom class*/
    void setCmd(char *s) {
        strcpy(cmd, s);
    }
    
//    void print() {
//        for (unsigned int j = 0;tokens[j] != 0;++j) {
//            printf("%s\n",tokens[j]);
//        }
//    }
    //parse single command into tokens
    void parse() {
        count = 0;
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
//        flag = 1;      // identify that the command execute successfully(=1) or not(=0)
        int status;
        char stre[10];
        strcpy(stre, "exit");

	    // If single command is "exit" then exit 
        if (strcmp(tokens[0], stre) == 0 && count == 1) return -1;
		// for test commmand
        int flag=0;
		if (strcmp(tokens[0],"test") == 0 || strcmp(tokens[0], "[") == 0) {
				if (strcmp(tokens[0], "[") == 0 && (tokens[3] == NULL || strcmp(tokens[3], "]" ) != 0)) {
					cout<<"Wrong test command!"<<endl;
					return 0;
				}
				struct stat sb;
				if (stat(tokens[2], &sb) == -1) {
					cout<<"(False)"<<endl;
					return 0;
				}
				if ((sb.st_mode & S_IFMT) == S_IFDIR) {
					flag=1;
				}
				if ((sb.st_mode & S_IFMT) == S_IFREG) {
					flag=2;
				}
				if ((flag == 1 || flag == 2) && strcmp(tokens[1], "-e") == 0) {
					cout<<"(True)"<<endl;
					return 1;
				}
				if (flag == 1 && strcmp(tokens[1], "-d") == 0) {	
					cout<<"(True)"<<endl;
					return 1;
				}
				if (flag == 2 && strcmp(tokens[1], "-f") == 0) {
					cout<<"(True)"<<endl;
					return 1;
				}
				cout<<"(False)"<<endl;		
				return 0;
		}	
        //create pipe
        int fd[2];
        int result;
        int readbuffer;
        int message = 1;
        result = pipe(fd);       
        if (result < 0) {
            perror("ERROR: Pipe error\n");
            return 0;
        }

	    // Execute the single command
        pid = fork();

	    if (pid < 0) {
            perror("ERROR: Forking child process failure\n");
            //exit(EXIT_FAILURE);
            return 0;
        }
        else if (pid == 0) {
            execvp(tokens[0], tokens);
            perror("ERROR:Execution failure\n");
            //close the input side of pipe and send execution message
            close(fd[0]);
            message = 0;
            write(fd[1], &message, sizeof(message));
            exit(EXIT_FAILURE);
        }
        else {
            //close output side of pipe and read in a string
            close(fd[1]);
            while (wait(&status) != pid);
            read(fd[0], &readbuffer, sizeof(readbuffer));
            if (readbuffer == 0) return 0;
            return 1;
        }
//        return flag;
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
        tcom = strtok(tcmdl, "|&;)(");
//        cout<<"size of comms:"<<coms.size()<<endl;
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
//            coms[i].print();
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
            else if (*it == '(') {
				string t="(";
				connectors.push_back(t);
            }
            else if (*it == ')') {
				string t=")";
				connectors.push_back(t);
            }
        }
    }
    /* Execute the command according to the connectors. 
  Each single command follows a connector except the first one.*/
    int execute() {
        int result;    // Store the status of the execution of a single command  0 = fail, 1 = succeed -1 = exit
        result = coms[0].execute();
//        cout<<"result of command 0:"<<result<<endl;
        if (result == -1) exit(0);
        unsigned int i = 0;
        unsigned int j = 0;
        int flag;
//        cout<<"connectors.size()"<<connectors.size()<<endl;
        while (i < connectors.size()&&result!=-1) {
            if (connectors[i] == "||" && result == 1) {
                i++;
                j++;
                if (connectors[i] == "(" ) {
                	flag=1;
					while (flag > 0) {
						i++;
						j++;
						if ( connectors[i] == "(" ) { 
							flag++;
							j--;
						}
						if ( connectors[i] == ")" ) {
							flag--;
							j--;
						}
					}
                }
            }
            else if (connectors[i] == "&&" && result == 0) {
                i++;
                j++;
				if (connectors[i] == "(" ) {
					flag=1;
					while (flag > 0) {
						i++;
						j++;
						if ( connectors[i] == "(" ) {
							flag++;
							j--;
						}
						if ( connectors[i] == ")" ) {
							flag--;
							j--;
						}
					}
				}

            }
            else if (connectors[i] == "(" || connectors[i] == ")") {
				i++;
				while (connectors[i] == "(" || connectors[i] == ")") i++;
            }
            else {
                ++i;
                j++;
//                result = 1;
                result = coms[j].execute();
//                cout<<"result of command "<<i<<":"<<result<<endl;
            }
            if (result == -1) break;
        }
        if (result == -1) return -1;
        return 1;
    }
};
/*
class PreCom: public Command {
protected:
	char cmdl[1000];
public:
	void clear () {
	}

	void setCmd (char* s){
		strcpy(cmdl,s);
	}
	
	void parse () {
    // Delete the comment from command line                                                                                                                                     
	    char* tmp;
		tmp = strchr(cmdl, '#');
		char *cmdll;
		if (tmp != NULL) {
			cmdll = new char[strlen(cmdl) - strlen(tmp)];
			strncpy(cmdll, cmdl, strlen(cmdl) - strlen(tmp));
		}
		else
		{
			cmdll = new char[strlen(cmdl)];
			strcpy(cmdll,cmdl);
		}
		string scmd(cmdll);
		string::iterator it;
		char cmd[1000];
		while (i<scmd.size()) {
			if (scmd[i] == '(') {
				j=i-1;
				while (j > 0 && scmd[j] != '|' && scmd[j] != '&') {
					j--;
				}
				if (j > 0) {
					string con;,
					con[0]=scmd[j];
					con[1]=scmd[j-1];
					con[2]=0;
					connectors.push_back(con);
					con=scmd.substr(
				}
			}
			if (*it == ')') {
				cmds.push_back(cmd);
				cmd[0]=0;
				flag=1;
			}
			if (flag && (*it == '|' || *it == '&')) {
				it++;
				char con[5];
				
			}
		}
	}

	int execute () {
	}
};
*/

#endif
