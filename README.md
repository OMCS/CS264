CS264
=====

Robotics Assignment One - Navigating an Occupancy Grid

# Usage

A makefile is provided, if you are running on Linux, simply run `make linux` in the parent directory, if you are using Mac OS X then `make` will suffice.

This will create a file at `bin/robotControl` which can then be executed once a suitable Player server is running. To start this server you can run `startServer.sh`, it will run in the background.

You will be asked to input grid coordinates, the robot will then print out the path it is attempting to traverse at each stage before reaching the goal. Note that there is error checking on the input for illegal values and occupied grid spaces.
