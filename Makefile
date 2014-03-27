all: 
	@mkdir -p ./bin
	clang++ --std=c++0x --stdlib=libc++ -I ./include -o ./bin/robotControl `pkg-config --cflags playerc++` src/robot.cpp src/grid.cpp src/node.cpp `pkg-config --libs playerc++` 
debug:
	@rm -rf ./bin/
	@mkdir -p ./bin
	clang++ -O0 -g --std=c++0x --stdlib=libc++ -I ./include -o ./bin/robotControl `pkg-config --cflags playerc++` src/robot.cpp src/grid.cpp src/node.cpp `pkg-config --libs playerc++` 
linux:
	@mkdir -p ./bin
	g++ -std=c++0x -I ./include -o ./bin/robotControl `pkg-config --cflags playerc++` src/robot.cpp src/grid.cpp src/node.cpp `pkg-config --libs playerc++` 
clean:
	@rm -rf ./bin/

