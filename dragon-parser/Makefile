

main: commandline parsing typenodes statements
	g++ -O0 -march=native -g --std=c++17  -o dragon-parser *.o

commandline: src/main/commandline/*.cpp
	g++ -O0 -march=native -g --std=c++17 -c src/main/commandline/*.cpp

parsing: src/main/parsing/*.cpp
	g++ -O0 -march=native -g --std=c++17  -c src/main/parsing/*.cpp

typenodes: src/main/parsing/typenodes/*.cpp
	g++ -O0 -march=native -g --std=c++17 -c src/main/parsing/typenodes/*.cpp

statements: src/main/parsing/statements/*.cpp
	g++ -O0 -march=native -g --std=c++17 -c src/main/parsing/statements/*.cpp
