#! /bin/bash 

here=$(pwd)
 
PIONEER3DX_DRIVERS_WS=$here/pioneer3dx
PIONEER3DX_SIMULATION_WS=$here/pioneer3dx_simulation
PIONEER3DX_2dSLAM_WS=$here/pioneer3dx_2dslam
PIONEER3DX_MAP_WS=$here/pioneer3dx_map_and_nav

cd $PIONEER3DX_DRIVERS_WS
catkin_make
cd $PIONEER3DX_SIMULATION_WS
catkin_make
cd $PIONEER3DX_2dSLAM_WS
catkin_make
cd $PIONEER3DX_MAP_WS
catkin_make
cd $here
