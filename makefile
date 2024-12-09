make: all

all: clean compile link run

clean:
	rm -f ./bin/* bin/simulation

compile : src/Auxiliary.cpp src/main.cpp src/Simulation.cpp src/Settlement.cpp src/Facility.cpp src/selectionpolicy.cpp src/Action.cpp src/Plan.cpp
	g++ -c -Wall -g -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -c -Wall -g -Iinclude -o bin/main.o src/main.cpp
	g++ -c -Wall -g -Iinclude -o bin/Simulation.o src/Simulation.cpp
	g++ -c -Wall -g -Iinclude -o bin/Settlement.o src/Settlement.cpp
	g++ -c -Wall -g -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -c -Wall -g -Iinclude -o bin/selectionpolicy.o src/selectionpolicy.cpp
	g++ -c -Wall -g -Iinclude -o bin/Action.o src/Action.cpp
	g++ -c -Wall -g -Iinclude -o bin/Plan.o src/Plan.cpp


link : bin/main.o bin/Auxiliary.o bin/Simulation.o bin/Settlement.o bin/Action.o bin/Plan.o bin/Facility.o
	g++ -o bin/simulation bin/main.o bin/Auxiliary.o bin/Simulation.o bin/Settlement.o bin/Facility.o bin/selectionpolicy.o bin/Action.o bin/Plan.o

plan:
	g++ -c -Wall -g -Iinclude -o bin/Plan.o src/Plan.cpp

run : bin/simulation
	bin/simulation config_file.txt