#! /bin/bash 

here=$(pwd)

DRIVERS=$here/pioneer3dx
SIMULATION=$here/pioneer3dx_simulation
SLAM=$here/pioneer3dx_2dslam
MAP_NAV=$here/pioneer3dx_map_and_nav

source $DRIVERS/devel/setup.bash --extend
source $SIMULATION/devel/setup.bash --extend
source $SLAM/devel/setup.bash --extend
source $MAP_NAV/devel/setup.bash --extend
