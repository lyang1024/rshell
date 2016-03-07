echo "Command 1 : (echo A && echo B) || (echo C && echo D)
Command 2: (echo A) && (echo B)
Command 3: (echo A || (echo B && echo C)) && (echo D || echo E && echo H)
Command 4: (fail command1 || (fail command2 && (echo A || echo B))) || ([ -f src/Command.h ] && test -d src/Command.h)"
../bin/rshell < precedence1
