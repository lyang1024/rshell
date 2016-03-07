#!/bin/sh
echo "Command 1: test src && test -e src && test -d src && test -f src && test -f src/Command.h"  
echo "Command 2: [ src ] && [ -e src ] && [ test -d src ] && [ -f src ] && [ -f src/Command.h ]"
bin/rshell < ./tests/test1

