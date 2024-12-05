all: link

link: compile
	@echo "Linking object files"
	g++ -o bin/main bin/main.o bin/Auxiliary.o

compile: src/main.cpp src/Auxiliary.cpp
	@echo "Compiling source code"
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

clean:
	@echo "Cleaning bin directorty"
	rm -f bin/*