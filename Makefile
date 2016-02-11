all: rshell

rshell:
	mkdir bin
	g++ -Wall -Werror -ansi -pedantic main.cpp -o bin/rshell

clean:
	rm -rf bin/
