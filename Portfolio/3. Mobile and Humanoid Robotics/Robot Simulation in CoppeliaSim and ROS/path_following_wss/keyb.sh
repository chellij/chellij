#! /bin/bash

here=$(pwd)
 
KEYB=$here/pioneer3dx/src/turtlebot_teleop/launch

source source.bash 
cd $KEYB
roslaunch keyboard_teleop.launch

