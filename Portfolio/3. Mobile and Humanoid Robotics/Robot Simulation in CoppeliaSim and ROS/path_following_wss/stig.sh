#! /bin/bash

here=$(pwd)

STIG=$here/pioneer3dx_map_and_nav/src/stigmergy_planner/launch

source source.bash 
cd $STIG
roslaunch stigmergy_planner.launch

