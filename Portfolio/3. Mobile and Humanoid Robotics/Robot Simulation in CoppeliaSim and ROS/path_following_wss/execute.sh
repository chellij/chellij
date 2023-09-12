#! /bin/bash 

here=$(pwd)

DRIVERS=$here/pioneer3dx
SIMULATION=$here/pioneer3dx_simulation
SLAM=$here/pioneer3dx_2dslam
MAP_NAV=$here/pioneer3dx_map_and_nav

chmod +x copp.sh
chmod +x del.sh
chmod +x install.sh
chmod +x keyb.sh
chmod +x make.sh
chmod +x octo.sh
chmod +x run.sh
chmod +x run_path.sh
chmod +x rviz.sh
chmod +x slam.sh
chmod +x source.bash
chmod +x stig.sh
chmod +x traj.sh

cd $DRIVERS/src/float32multiarray_to_pointcloud2/launch
chmod +x float32multiarray_to_pointcloud2.launch

cd $DRIVERS/src/turtlebot_teleop/launch
chmod +x keyboard_teleop.launch

cd $SLAM/src/pioneer3dx_gmapping/launch
chmod +x pioneer3dx_gmapping.launch

cd $MAP_NAV/src/pioneer3dx_octomap/launch
chmod +x pioneer3dx_octomap.launch

cd $MAP_NAV/src/stigmergy_planner/launch
chmod +x stigmergy_planner.launch

cd $MAP_NAV/src/trajectory_control/launch
chmod +x trajectory_controller.launch

cd $SIMULATION/src/coppeliasim_p3dx_sim/launch
chmod +x coppeliasim_p3dx_launch.launch
chmod +x coppeliasim_p3dx_sim.launch
chmod +x coppeliasim_p3dx_sim_office.launch

 
