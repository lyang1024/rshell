echo "Test command 1 : exit"
bin/rshell < ./test/exitcmd
echo "Test command 2 : test exit && [ exit ] && exit "
bin/rshell < ./test/exitcmd3
echo "Test command 3 : (echo A && echo B) || (echo C && exit) && exit"
bin/rshell < ./test/exitcmd4
