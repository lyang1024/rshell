# rshell

Provide a terminal interface for users to input commands. Run commands by forking child process and using execvp() to execute.
Shell prompt provide information of the current logged in user and the hostname of the machine the user is logged into.
The Test Command:
For this assignment we add the test command to our rshell, as well as its symbolic equivalent[]. This command can detect whether the file/directory exists or not.
The Precedence Operators:
we implement parentheses () as precedence operators in our rshell. The parentheses() operators are used to change the precedence of the returns of commands, connectors, and chain of connectors.

##License Information

Copyright (c) 2016, Liuqing Yang, Xuan Zhuang
All rights reserved.

All files are licensed under GPL.

https://github.com/lyang1024/rshell/blob/master/LICENSE

##Install and Run Program

Open a terminal and run the following commands.

1. Clone repository.
'git clone https://github.com/lyang1024/rshell.git
2. Get into the project directory.
'cd rshell'
3. Get tag
'git checkout hw2'
4. Compile and create executable file
'make'
5. Execute rshell
'bin/rshell'

##Error Message

When a command fails, rshell will return an error message using perror and continue the rest of commands. 

##Testing Method

1) Testing method for test folder:
1. Include test cases of different type of commands in seperate bash file.
2. All test files are in the "test" directory.
3. Run "bin/rshell" first then test the bash files. E.g. Type in "./test/single_command.sh" after starting rshell to test commands included in this file.
4. Special case: the bash file "exit.sh" needs to be run outside rshell environment. E.g. "./test/exit.sh"

2) Testing method for tests folder:
1. include test cases of different type of commands in seperate bash file.
2. All test files are in the "tests" directory.
3. Run test files outside rshell environment. E.g. "./tests/test_test.sh"

##Limitations and Bugs

1. Using connectors without arguments in between(e.g. &&&&) will cause segmentation fault.
2. Running commands that have character(s) which is(are) also included in connectors will not be recognized and cause an error.
3. Colors for files, directories, etc. are not supported.
4. Right and left arrow cannot move cursor when inputting commands.
5. Up and down arrow cannot get previous commands. 
6. If the string after echo includes connectors, an execution fault will occur. 
7. When using "[" and "]" to test direcotry or files, we need to use space to split the command with "[" and "]". E.g. "[ -e src/Command.h ]".
8. The rshell can not detect whether the brackets "(" and ")" match or not.
