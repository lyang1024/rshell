#ifndef rshell_Command_h
#define rshell_Command_h
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string>
#include<vector>
#include<cstring>
#include<sstream>
#include<stdlib.h>
#include<iostream>
using namespace std;

//Base class
class Command {

    public:
    Command(){};
    virtual void parse() = 0;
    virtual int execute() = 0;
    virtual void print() = 0;
//    virtual void clear() = 0;
    virtual void setCmd(string s) = 0;
};

//single command class
class SingleCom: public Command {
    protected:
    std::string cmd;
    std::vector<string> tokens;

    public:
    SingleCom() : Command(){};
//    void clear() {
//    }
//
    void setCmd(string s) {
        cmd = s;
    }

    void print() {
        cout<<cmd<<endl;
    }
    void parse() {
        //delete () from command string
        while(true) {
            std::string::iterator it = cmd.begin();
            for (;it != cmd.end(); ++it) {
                if (*it == '(' || *it == ')') {
                    cmd.erase(it);
                    break;
                }
            }
            if (it == cmd.end()) break;
        }
        char *cmdc = new char[cmd.length() + 1];
        strcpy(cmdc, cmd.c_str());
        char *token = strtok(cmdc, " ");
        while (token != NULL) {
            std::string tmps(token);
            tokens.push_back(tmps);
            token = strtok(NULL, " ");
        }
        delete [] cmdc;
        return;
    }

    int execute() {
        pid_t pid;
        int status;
        if (tokens.size() == 1 && tokens.at(0) == "exit") return -1;
        else if (tokens.size() == 0) {
            cout<<"empty command!"<<endl;
            return 0;
        }
        //convert string to char
        char *ctokens[100];
        unsigned int n = 0;
        for (;n < tokens.size();++n) {
            string ttmp = tokens.at(n);
            ctokens[n] = new char[ttmp.length() + 1];
            strcpy(ctokens[n],ttmp.c_str());
        }
        ctokens[n] = NULL;
        int flag = 0;
        if (tokens.at(0) == "test" || tokens.at(0) == "[") {
            if ((tokens.at(0) == "[" && tokens.size() == 4) || (tokens.at(0) == "test" && tokens.size() == 3)) {
                //string tmpts = tokens.at(2);
                //char *tmptc = new char[tmpts.length() + 1];
                //strcpy(tmptc,tmpts.c_str());
                struct stat sb;
                if (stat(ctokens[2], &sb) == -1) {
                    cout<<"(False)"<<endl;
                    return 1;
                }
                else {
                    if (S_ISDIR(sb.st_mode)) flag = 1;
                    else if (S_ISREG(sb.st_mode)) flag = 2;

                    if (flag == 1) {
                        if (tokens.at(1) == "-e" || tokens.at(1) == "-d") {
                            cout<<"(True)"<<endl;
                            return 1;
                        }
                    }
                    else if (flag == 2) {
                        if (tokens.at(1) == "-e" || tokens.at(1) == "-f") {
                            cout<<"(Ture)"<<endl;
                            return 1;
                        }
                    }
                    cout<<"(False)"<<endl;
                    return 1;
                }
                //delete [] tmptc;
            }
            else if ((tokens.at(0) == "[" && tokens.size() == 3) || (tokens.at(0) == "test" && tokens.size() == 2)) {
                //string tmpts = tokens.at(1);
                //char *tmptc = new char[tmpts.length() + 1];
                //strcpy(tmptc,tmpts.c_str());
                struct stat sb;
                if (stat(ctokens[1], &sb) == -1) {
                    cout<<"(False)"<<endl;
                    return 1;
                }
                else {
                    if (S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode)) {
                        cout<<"(True)"<<endl;
                        return 1;
                    }
                    else {
                        cout<<"(False)"<<endl;
                        return 1;
                    }
                }
            }
        }
        int fd[2];
        int result;
        int readbuffer = 1;
        int message = 1;
        result = pipe(fd);
        if (result < 0) {
            perror("ERROR: Pipe error\n");
            return 0;
        }

        //Execute using child process
        pid = fork();
        if (pid <0 ) {
            perror("ERROR: Forking child process failure\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            execvp(ctokens[0], ctokens);
            perror("ERROR: Execution failure\n");
            //close the input side of pipe and send execution message
            close(fd[0]);
            message = 0;
            write(fd[1], &message, sizeof(message));
            exit(EXIT_FAILURE);
        }
        else {
            //parent process
            //close output side of pipe and read in a string
            close(fd[1]);
            while (wait(&status) != pid);
            read(fd[0], &readbuffer, sizeof(readbuffer));
            if (readbuffer == 0) return 0;
            else return 1;
        }
        for (unsigned int m = tokens.size() - 1;m >= 1;--m) {
            delete [] ctokens[m];
        }
		delete [] ctokens[0];
    }
};

//multi-command class
class MultiCom: public Command {
    private:
    vector<SingleCom> coms;
    vector<string> connectors;
    std::string commandline;

    public:
    MultiCom() : Command() {};
    void setCmd(string s) {
        commandline = s;
    }
    void print()
    {
        cout<<"commands:"<<endl;
        for (unsigned int p = 0;p < coms.size();++p) {
            coms.at(p).print();
        }
        cout<<"connectors:"<<endl;
        for (unsigned int q = 0;q < connectors.size();++q) {
            cout<<connectors.at(q)<<endl;
        }
    }
    void parse() {
        //remove comment
        std::string comment("#");
        size_t com_pos = commandline.find(comment);
        string cmdline;
        if (com_pos != std::string::npos) {
            cmdline = commandline.substr(0,com_pos);
        }
        else {
            cmdline = commandline;
        }
        
        //break into single commands
        char *cmdlinec;
        char *cmdtoken;
        cmdlinec = new char[cmdline.size()];
        strcpy(cmdlinec,cmdline.c_str());
        cmdtoken = strtok(cmdlinec,"|&;");
        while (cmdtoken != NULL) {
            std::string tmptoken(cmdtoken);
            SingleCom sc;
            sc.setCmd(tmptoken);
            coms.push_back(sc);
            cmdtoken = strtok(NULL, "|&;");
        }
        //parse each single command
        for (unsigned int i = 0; i<coms.size(); i++) {
            coms[i].parse();
        }
        //find connectors
        for (string::iterator it = cmdline.begin(); it != cmdline.end(); ++it) {
            if (*it == '|') {
                ++it;
                if (*it == '|') {
                    string tmpcon = "||";
                    connectors.push_back(tmpcon);
                }
                else {
                    cout<<"wrong input format!"<<endl;
                    return;
                }
            }
            //&&
            else if (*it == '&') {
                ++it;
                if (*it == '&') {
                    string tmpcon = "&&";
                    connectors.push_back(tmpcon);
                }
                else {
                    cout<<"wrong input format!"<<endl;
                    return;
                }
            }
            //;
            else if (*it == ';' || *it == '(' || *it == ')') {
                string tmpcon;
                stringstream ss;
                ss << *it;
                ss >> tmpcon;
                connectors.push_back(tmpcon);
            }
        }
        delete [] cmdlinec;
    }
    //execute
    int execute() {
//        this->print();
        int result = coms[0].execute();
        unsigned int i = 0;
        unsigned int j = 0;
        int count; //count the number of pair of brackets
        //int pair;
        while (i < connectors.size() && result != -1 && j < coms.size()) {
            if ((connectors[i] == "||" && result == 1) || (connectors[i] == "&&" && result == 0)) {
                ++i;
                ++j;
                if (i == connectors.size()) break;
                if (connectors[i] == "(") {
                    count = 1;
                    //pair = 0;
                   // ++i;
                    //++j; //the number of connectors could be larger than commands now
                    while (count > 0 && i < connectors.size()) {
						i++;
						if (connectors[i] == "(") {
                            ++count;
                        }
                        else if (connectors[i] == ")") {
                            --count;
                            //++pair;
                        }
                        else {
                            if( j < coms.size())
                                ++j;
                        }
                       // ++i; // in the last loop, i points at a new unanalyzed connector
                    }
                    //++j; //if not add, j points to the last command in the bracket
                }
            }
            else if ((connectors[i] == "(" || connectors[i] == ")") && i < connectors.size()) {
 //               cout<<"meet ( or )"<<endl;
                ++i;
//             cout<<"before loop"<<endl;
                while (i < connectors.size() && (connectors[i] == "(" || connectors[i] == ")")) ++i;
//                cout<<"after add i = "<<i<<endl;
            }
            else {
                if (i < connectors.size())
                    ++i;
                if (j < coms.size()) {
                    ++j;
                    result = coms[j].execute();
//                    cout<<"after this execute"<<endl;
//                    cout<<"i = "<<i<<", j = "<<j<<endl;
                }
            }
            if (result == -1) break;
//            cout<<"current i:"<<i<<endl;
//            cout<<"connectors size:"<<connectors.size()<<endl;
        }
//        cout<<"out of the loop"<<endl;
        if (result == -1) return -1;
        else return 1;
    }
};
#endif
