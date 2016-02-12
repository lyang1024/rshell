#!/bin/n/rshell
echo "Testing command 1 : exit"
exit
echo "Testing command 2 : ls -a && exit && cal"
ls -a && exit && calsh
echo "Testing command 3: exit fake && exit; cal"
exit fake && exit; cal
