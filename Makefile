all: 
	clang++ --std=c++0x --stdlib=libc++ -o ./BIN/robotControl `pkg-config --cflags playerc++` SRC/robot.cpp SRC/grid.cpp SRC/node.cpp `pkg-config --libs playerc++` 
clean:
	@rm -f ./BIN/*

