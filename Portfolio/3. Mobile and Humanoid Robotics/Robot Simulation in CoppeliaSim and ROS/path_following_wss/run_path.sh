#! /bin/bash

here=$(pwd)

source source.bash
gnome-terminal --tab -e roscore; gnome-terminal --tab -e ./copp.sh ; gnome-terminal --tab -e ./rviz.sh;  gnome-terminal --tab -e ./octo.sh ;  gnome-terminal --tab -e ./stig.sh ; gnome-terminal --tab -e ./traj.sh 

