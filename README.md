# rshell

Provide a terminal interface for users to input commands. Run commands by forking child process and using execvp() to execute.
Shell prompt provide information of the current logged in user and the hostname of the machine the user is logged into.


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
3. Compile and create executable file
'make'
4. Execute rshell
'bin/rshell'

##Limitations and Bugs

1. Using connectors without arguments in between(e.g. &&&&) will cause segmentation fault.
2. Running commands that have character(s) which is(are) also included in connectors will not be recognized and cause an error.
3. Colors for files, directories, etc. are not supported.
4. Right and left arrow cannot move cursor when inputting commands.
5. Up and down arrow cannot get previous commands. 

