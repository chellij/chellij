#! /bin/bash

here=$(pwd)
 
TRAJ=$here/pioneer3dx_map_and_nav/src/trajectory_control/launch

source source.bash 
cd $TRAJ
roslaunch trajectory_controller.launch

