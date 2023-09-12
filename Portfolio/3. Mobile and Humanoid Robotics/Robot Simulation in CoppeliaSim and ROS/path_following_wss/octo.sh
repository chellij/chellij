#! /bin/bash

here=$(pwd)
 
OCTO=$here/pioneer3dx_map_and_nav/src/pioneer3dx_octomap/launch

source source.bash 
cd $OCTO
roslaunch pioneer3dx_octomap.launch

