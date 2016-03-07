#!/bin/sh
echo "Test command 1 : ls -a && pwd"
ls -a && pwd
echo "Test command 2 : fail command && pwd"
fail command && pwd
echo "Test command 3 : ls -a || pwd"
ls -a || pwd
echo "Test command 4 : fail command || pwd"
fail command || pwd
echo "Test command 5 : ls -a; pwd"
ls -a; pwd
echo "Test command 6 : fail command; pwd"
fail command; pwd
echo "Test command 7 : cal; ls -a && pwd || echo world; echo hello"
cal; ls -a && pwd || echo world; echo hello
echo "Test command 8 : ls -a && fail command || pwd"
ls -a && fail command || pwd
echo "Test command 9 : fail command && ls -a || pwd"
fail command && ls -a || pwd
echo "Test command 10 :cal;echo hello&&mkdir test||echo world;git status"
cal;echo hello&&mkdir testdirectory||echo world;git status
 
