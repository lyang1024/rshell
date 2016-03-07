echo "Test command 1 : exit"
bin/rshell < exitcmd
echo "Test command 2 : ls -a && exit && cal"
bin/rshell < exitcmd1
echo "Test command 3 : exit fake && exit; cal && exit"
bin/rshell < exitcmd2
