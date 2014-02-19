all: SRC/robot.cpp
	g++ -o ./BIN/robotControl `pkg-config --cflags playerc++` SRC/robot.cpp `pkg-config --libs playerc++` -I /usr/local/include/player-3.0

