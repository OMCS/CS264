all: 
	@mkdir -p ./bin
	clang++ --std=c++0x --stdlib=libc++ -I ./include -o ./bin/robotControl `pkg-config --cflags playerc++` SRC/robot.cpp SRC/grid.cpp SRC/node.cpp `pkg-config --libs playerc++` 
debug:
	@rm -rf ./bin/
	@mkdir -p ./bin
	clang++ -O0 -g --std=c++0x --stdlib=libc++ -I ./include -o ./bin/robotControl `pkg-config --cflags playerc++` SRC/robot.cpp SRC/grid.cpp SRC/node.cpp `pkg-config --libs playerc++` 
clean:
	@rm -rf ./bin/

