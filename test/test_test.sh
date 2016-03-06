#!/bin/sh
echo "test src && test -e src && test -d src && test -f src && test -f src/Command.h"  
echo "[ src ] && [ -e src ] && [ test -d src ] && [ -f src ] && [ -f src/Command.h ]"
bin/rshell < p2_1

