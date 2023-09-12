#! /bin/bash

here=$(pwd)
 
SLAMMAP=$here/pioneer3dx_2dslam/src/pioneer3dx_gmapping/launch

source source.bash 
cd $SLAMMAP
roslaunch pioneer3dx_gmapping.launch

