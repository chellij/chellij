/*
 * TrajectoryControlActionServer.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: alcor
 */

#include <math.h>
#include <trajectory_control/TrajectoryControlActionServer.h>


TrajectoryControlActionServer::TrajectoryControlActionServer():
n_("~"),
linear_vel(0),
angular_vel(0)
{

        global_frame_id = getParam<std::string>(n_, "global_frame_id", "/map");
	odom_frame_id = getParam<std::string>(n_, "odom_frame_id", "/odom");
        robot_frame_id = getParam<std::string>(n_, "robot_frame_id", "/base_link");
        
        trajectory_control_action_server_name = getParam<std::string>(n_, "trajectory_control_action_server_name", "trajectory_control_action_server");
        as_ = new actionlib::SimpleActionServer<trajectory_control_msgs::TrajectoryControlAction>(node, trajectory_control_action_server_name, boost::bind(&TrajectoryControlActionServer::executeCallback, this, _1), false);
        ac_ = new actionlib::SimpleActionClient<trajectory_control_msgs::TrajectoryControlAction>(trajectory_control_action_server_name, true);
        //0.331 distance (in m) between centre of wheels
        //0.195 diameter_wheel
        
        robot_width = getParam<double>(n_, "robot_width", 0.354); 
	 displacement = getParam<double>(n_, "displacement", 0.2);
	 vel_reference = getParam<double>(n_, "vel_reference", 0.15);
         max_wheel_vel = getParam<double>(n_, "max_wheel_vel", 0.2);
	 
         imu_odom_topic = getParam<std::string>(n_, "imu_odom_topic", "/imu_odom");
         imu_odom_sub = node.subscribe(imu_odom_topic,1,&TrajectoryControlActionServer::imuOdomCallback,this);
         
	 tracks_vel_cmd_topic = getParam<std::string>(n_, "tracks_vel_cmd_topic", "/tracks_vel_cmd");
         //tracks_vel_cmd_pub = node.advertise<nifti_robot_driver_msgs::TracksStamped>(tracks_vel_cmd_topic,1);
         tracks_vel_cmd_pub = node.advertise<geometry_msgs::Twist>(tracks_vel_cmd_topic,1);
	 
         k1 = getParam<double>(n_, "gain_k1", 0.3);
	 k2 = getParam<double>(n_, "gain_k2", 0.3);
	 delay = getParam<double>(n_, "delay", 10);
	 
         global_path_topic = getParam<std::string>(n_, "global_path_topic","/global_path");
         global_path_ = node.advertise<nav_msgs::Path>(global_path_topic,1);
         
	 local_path_topic = getParam<std::string>(n_, "local_path_topic", "/local_path");
         local_path_ = node.advertise<nav_msgs::Path>(local_path_topic,1);

	 robot_path_topic = getParam<std::string>(n_, "robot_path_topic", "/robot_path");
         nav_path_sub = node.subscribe(robot_path_topic,1,&TrajectoryControlActionServer::pathCallBack,this);

	 while(!getRobotPose(current_real_robot_pose_odom,real_robot_pose_map,from_odom_to_map))
	 {
		 ROS_INFO("Waiting for transformation");
	 }

	 as_->start();

	 global_path.header.frame_id = global_frame_id;
	 global_path.header.stamp = ros::Time::now();

	 local_path.header.frame_id = global_frame_id;
	 local_path.header.stamp = ros::Time::now();

	 realRobotPoseB(displacement,real_robot_pose_map,real_robot_poseB_map);
}


TrajectoryControlActionServer::~TrajectoryControlActionServer()
{
}




void TrajectoryControlActionServer::odomMsgToStampedTransform(nav_msgs::Odometry pose_odometry, tf::StampedTransform& pose_stamped)
{
	pose_stamped.stamp_ = pose_odometry.header.stamp;
	pose_stamped.frame_id_ = pose_odometry.header.frame_id;
	pose_stamped.child_frame_id_ = pose_odometry.child_frame_id;

	tf::Vector3 v;
	v.setX(pose_odometry.pose.pose.position.x);
	v.setY(pose_odometry.pose.pose.position.y);
	v.setZ(pose_odometry.pose.pose.position.z);

	pose_stamped.setOrigin(v);

	tf::Quaternion q;
	q.setX(pose_odometry.pose.pose.orientation.x);
	q.setY(pose_odometry.pose.pose.orientation.y);
	q.setZ(pose_odometry.pose.pose.orientation.z);
	q.setW(pose_odometry.pose.pose.orientation.w);

	pose_stamped.setRotation(q);
}

void TrajectoryControlActionServer::realRobotPoseB(double displacement, tf::StampedTransform real_robot_pose, tf::StampedTransform& real_robot_poseB)
{
	real_robot_poseB.frame_id_ = real_robot_pose.frame_id_;
	real_robot_poseB.stamp_ = real_robot_pose.stamp_;
	real_robot_poseB.child_frame_id_ = real_robot_pose.child_frame_id_;

	tf::Vector3 v;
	double roll, pitch, yaw;
	real_robot_pose.getBasis().getRPY(roll,pitch,yaw);

	v.setX(real_robot_pose.getOrigin().getX() + displacement*cos(yaw));
	v.setY(real_robot_pose.getOrigin().getY() + displacement*sin(yaw));
	v.setZ(real_robot_pose.getOrigin().getZ());
	real_robot_poseB.setOrigin(v);

	real_robot_poseB.setRotation(real_robot_pose.getRotation());
}

bool TrajectoryControlActionServer::getRobotPose(tf::StampedTransform& robot_pose_odom, tf::StampedTransform& robot_pose_map, tf::StampedTransform& from_odom_to_map) {

	if(tf_.waitForTransform(global_frame_id,robot_frame_id,ros::Time(),ros::Duration(1.0)))
	{
		try
		{
			tf_.lookupTransform(global_frame_id,robot_frame_id,ros::Time(),robot_pose_map);
			tf_.lookupTransform(odom_frame_id,robot_frame_id,ros::Time(),robot_pose_odom);
			tf_.lookupTransform(global_frame_id,odom_frame_id,ros::Time(),from_odom_to_map);
		}
		catch(tf::LookupException& ex) {
			ROS_INFO("No Transform available Error looking up robot pose: %s\n", ex.what());
			return false;
		}
		catch(tf::ConnectivityException& ex) {
			ROS_INFO("Connectivity Error looking up robot pose: %s\n", ex.what());
			return false;
		}
		catch(tf::ExtrapolationException& ex) {
			ROS_INFO("Extrapolation Error looking up robot pose: %s\n", ex.what());
			return false;
		}

		return true;
	}
	else
	{
		ROS_INFO("Transformation is not available");
		return false;
	}
}

void TrajectoryControlActionServer::imuOdomCallback(const nav_msgs::OdometryConstPtr& msg)
{
	odomMsgToStampedTransform(*msg,current_real_robot_pose_odom);

	double yaw = tf::getYaw(current_real_robot_pose_odom.getRotation());

	if(tf_.waitForTransform(global_frame_id,odom_frame_id,msg->header.stamp,ros::Duration(0.05)))
	{
		try
		{
			tf_.lookupTransform(global_frame_id,robot_frame_id,msg->header.stamp,real_robot_pose_map);
			tf_.lookupTransform(global_frame_id,odom_frame_id,msg->header.stamp,from_odom_to_map);
		}

		catch(tf::LookupException& ex) {
			ROS_INFO("No Transform available Error looking up robot pose: %s\n", ex.what());
		}
		catch(tf::ConnectivityException& ex) {
			ROS_INFO("Connectivity Error looking up robot pose: %s\n", ex.what());
		}
		catch(tf::ExtrapolationException& ex) {
			ROS_INFO("Extrapolation Error looking up robot pose: %s\n", ex.what());
		}
	}
	else
	{

		tf::Transform transformation = from_odom_to_map * current_real_robot_pose_odom;
		real_robot_pose_map.stamp_ = current_real_robot_pose_odom.stamp_;
		real_robot_pose_map.setBasis(transformation.getBasis());
		real_robot_pose_map.setOrigin(transformation.getOrigin());
		real_robot_pose_map.setRotation(transformation.getRotation());
	}

	realRobotPoseB(displacement,real_robot_pose_map,real_robot_poseB_map);
}

void TrajectoryControlActionServer::pathCallBack(const nav_msgs::PathConstPtr &msg)
{
		ac_->waitForServer();
		//ROS_INFO("Trajectory Control is alive");
		trajectory_control_msgs::TrajectoryControlGoal track_goal;
		track_goal.path = *msg;
		ac_->sendGoal(track_goal);
}

void TrajectoryControlActionServer::resampleAndSmoothPath(const nav_msgs::Path& path_in, nav_msgs::Path& path_out)
{
    double vel = vel_reference;
    ros::Rate rate(delay);
    double time_step = rate.expectedCycleTime().nsec / 1e9; // durata dell'intervallo temporale

    path_out.header.frame_id = path_in.header.frame_id;
    path_out.header.stamp = path_in.header.stamp;

    double dist_step;

    geometry_msgs::PoseStamped current;
    current.header = path_in.poses[0].header;
    current.pose.position = path_in.poses[0].pose.position; // start from first point 

    geometry_msgs::PoseStamped new_point;

    double current_dist_to_next;
    double yaw;
    double offset = 0.0;
    bool is_change_point = false;

    int i = 1; // this is the index of next point 
    //while (i < (path_in.poses.size() - 1))
    while (i < path_in.poses.size())
    {
        //if (current.pose.position.x == path_in.poses[i].pose.position.x && current.pose.position.y == path_in.poses[i].pose.position.y)
        if (FEQUAL(current.pose.position.x, path_in.poses[i].pose.position.x, 1e-4) && FEQUAL(current.pose.position.y, path_in.poses[i].pose.position.y, 1e-4))
        {
            i++; // get the new point in the list 
        }
        else
        {
            if (is_change_point)
            {
                dist_step = fabs(vel * time_step - offset); // get a shorter step 
                current_dist_to_next = sqrt(pow(path_in.poses[i].pose.position.x - new_point.pose.position.x, 2) + pow(path_in.poses[i].pose.position.y - new_point.pose.position.y, 2));
                //ROS_INFO("changePoint current distance [%f]",current_dist);
                //ROS_INFO("changePoint distance [%f]",distance);
                //ROS_INFO("Offset: [%f]",offset);
                //ROS_INFO_STREAM("change point event");

                yaw = atan2(path_in.poses[i].pose.position.y - new_point.pose.position.y, path_in.poses[i].pose.position.x - new_point.pose.position.x);
                current.pose.orientation = tf::createQuaternionMsgFromYaw(yaw); // added 
            }
            else
            {
                dist_step = vel * time_step;
                current_dist_to_next = sqrt(pow(path_in.poses[i].pose.position.x - current.pose.position.x, 2) + pow(path_in.poses[i].pose.position.y - current.pose.position.y, 2));

                yaw = atan2(path_in.poses[i].pose.position.y - current.pose.position.y, path_in.poses[i].pose.position.x - current.pose.position.x);
                current.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);

                offset = 0.0;
            }
            //ROS_INFO(" offset [%f]",offset);
            //ROS_INFO("distance [%f]",distance);
            //ROS_INFO("curr distance [%f]",current_dist);
            if (current_dist_to_next >= dist_step)
            {
                geometry_msgs::PoseStamped temp;
                temp.header = path_in.poses[i].header;
                temp.pose.position.x = current.pose.position.x + (dist_step * cos(yaw));
                temp.pose.position.y = current.pose.position.y + (dist_step * sin(yaw));
                temp.pose.position.z = 0;

                if (is_change_point)
                {
                    yaw = atan2(temp.pose.position.y - current.pose.position.y, temp.pose.position.x - current.pose.position.x);
                    current.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);
                }

                temp.pose.orientation = current.pose.orientation;

                path_out.poses.push_back(current);
                //ROS_INFO_STREAM("added point: current_dist: "<< current_dist << " maggiore di distance: " << distance);
                //ROS_INFO_STREAM("new distance from added point: "<< sqrt(pow(in.poses[i].pose.position.x - temp.pose.position.x,2) + pow(in.poses[i].pose.position.y - temp.pose.position.y,2)));
                current.header = temp.header;
                current.pose = temp.pose;
                is_change_point = false;
                //add to global_pose
            }
            else
            {
                // the dist_step brings us beyond path_in.poses[i]
                // => we take path_in.poses[i] as intermediate point 
                // => we store the offset and we recover the missed distance in the next iteration 
                offset += fabs(current_dist_to_next - dist_step); // absolute value!
                new_point.header = path_in.poses[i].header;
                new_point.pose = path_in.poses[i].pose;
                is_change_point = true;

                //if(i == (path_in.poses.size() - 1)){}

                i++; // get the new point in the list 
            }

        }
        //ROS_INFO_STREAM("Index "<<i<< "");
    }

    ROS_INFO("Number of points input path [%ld]", path_in.poses.size());
    ROS_INFO("Number of points exe path [%ld]", path_out.poses.size());

    /*
    for(int i=1;i<global_plan.poses.size()-1;i++){

            global_plan.poses[i].pose.position.x = 0.3*global_plan.poses[i-1].pose.position.x + 0.4*global_plan.poses[i].pose.position.x + 0.3*global_plan.poses[i+1].pose.position.x;
            global_plan.poses[i].pose.position.y = 0.3*global_plan.poses[i-1].pose.position.y + 0.4*global_plan.poses[i].pose.position.y + 0.3*global_plan.poses[i+1].pose.position.y;
    }
     */
}

void TrajectoryControlActionServer::filtering(nav_msgs::Path in, nav_msgs::Path& global_plan)
{

	double vel = vel_reference;
	ros::Rate rate(delay);
	double timestep = rate.expectedCycleTime().nsec/1e9; // durata dell'intervallo temporale

	global_plan.header.frame_id = in.header.frame_id;
	global_plan.header.stamp = in.header.stamp;

	double distance;

	geometry_msgs::PoseStamped current;
	current.header = in.poses[0].header;
	current.pose.position = in.poses[0].pose.position;


	double current_dist;
	double yaw;
	double offset=0.0;
	bool changePoint=false;
	geometry_msgs::PoseStamped newPoint;

	int i = 1;
	while(i < (in.poses.size()-1))
	{
		if(current.pose.position.x == in.poses[i].pose.position.x && current.pose.position.y == in.poses[i].pose.position.y){
			i++;
		}else{
			if(changePoint){
				distance =  fabs(vel*timestep-offset);
				current_dist = sqrt(pow(in.poses[i].pose.position.x - newPoint.pose.position.x,2) + pow(in.poses[i].pose.position.y - newPoint.pose.position.y,2));
				//ROS_INFO("changePoint current distance [%f]",current_dist);
				//ROS_INFO("changePoint distance [%f]",distance);
				//ROS_INFO("Offset: [%f]",offset);
				//ROS_INFO_STREAM("change point event");

				yaw = atan2(in.poses[i].pose.position.y - newPoint.pose.position.y,in.poses[i].pose.position.x - newPoint.pose.position.x);
			}else{
				distance = vel * timestep;
				current_dist = sqrt(pow(in.poses[i].pose.position.x - current.pose.position.x,2) + pow(in.poses[i].pose.position.y - current.pose.position.y,2));
				yaw = atan2(in.poses[i].pose.position.y - current.pose.position.y,in.poses[i].pose.position.x - current.pose.position.x);

				current.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);
				offset=0.0;
			}
			//ROS_INFO(" offset [%f]",offset);
			//ROS_INFO("distance [%f]",distance);
			//ROS_INFO("curr distance [%f]",current_dist);
			if(current_dist >= distance)
			{

				geometry_msgs::PoseStamped temp;
				temp.header = in.poses[i].header;
				temp.pose.position.x = current.pose.position.x + (distance * cos(yaw));
				temp.pose.position.y = current.pose.position.y + (distance * sin(yaw));
				temp.pose.position.z = 0;

				if(changePoint){
					yaw = atan2(temp.pose.position.y - current.pose.position.y,temp.pose.position.x - current.pose.position.x);
					current.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);
				}
				global_plan.poses.push_back(current);
				//ROS_INFO_STREAM("added point: current_dist: "<< current_dist << " maggiore di distance: " << distance);
				//ROS_INFO_STREAM("new distance from added point: "<< sqrt(pow(in.poses[i].pose.position.x - temp.pose.position.x,2) + pow(in.poses[i].pose.position.y - temp.pose.position.y,2)));
				current.header = temp.header;
				current.pose = temp.pose;
				changePoint=false;
				//add to global_pose
			}
			else
			{
				offset += fabs(current_dist - distance);
				newPoint.header = in.poses[i].header;
				newPoint.pose = in.poses[i].pose;
				changePoint=true;
				i++;
			}

		}
		//ROS_INFO_STREAM("Index "<<i<< "");
	}
	//ROS_INFO("Number of points [%lu]",in.poses.size());
	/*
	for(int i=1;i<global_plan.poses.size()-1;i++){

		global_plan.poses[i].pose.position.x = 0.3*global_plan.poses[i-1].pose.position.x + 0.4*global_plan.poses[i].pose.position.x + 0.3*global_plan.poses[i+1].pose.position.x;
		global_plan.poses[i].pose.position.y = 0.3*global_plan.poses[i-1].pose.position.y + 0.4*global_plan.poses[i].pose.position.y + 0.3*global_plan.poses[i+1].pose.position.y;
	}
	*/
}

void TrajectoryControlActionServer::buildUserDefinedTrajectory(double vel, geometry_msgs::PoseStamped in, geometry_msgs::Pose& poseB, geometry_msgs::Twist& velB)
{
	poseB = in.pose;

	double yaw = tf::getYaw(in.pose.orientation);

	in.header.stamp = ros::Time::now();
	global_path.poses.push_back(in);
	global_path_.publish(global_path);

	velB.linear.x = vel * cos(yaw);
	velB.linear.y = vel * sin(yaw);
}

void TrajectoryControlActionServer::getRobotCommands(double displacement, tf::StampedTransform robot_pose, double k1, double k2, geometry_msgs::Pose poseB, geometry_msgs::Twist velB, double& linear_vel, double& angular_vel)
{
	double roll, pitch, yaw;
	robot_pose.getBasis().getRPY(roll,pitch,yaw);

	double a = cos(yaw);
	double b = sin(yaw);
	double c = -sin(yaw)/displacement;
	double d = cos(yaw)/displacement;

	double u1 = velB.linear.x + k1 * (poseB.position.x - robot_pose.getOrigin().getX());
	double u2 = velB.linear.y + k2 * (poseB.position.y - robot_pose.getOrigin().getY());

	//double u1 = k1 * (poseB.position.x - robot_pose.getOrigin().getX());
	//double u2 = k2 * (poseB.position.y - robot_pose.getOrigin().getY());

	linear_vel = a * u1 + b * u2;
	angular_vel = c * u1 + d * u2;

	geometry_msgs::PoseStamped poseR;
	poseR.header.frame_id = robot_pose.frame_id_;
	poseR.header.stamp = robot_pose.stamp_;
	poseR.pose.position.x = robot_pose.getOrigin().getX() - displacement*cos(yaw);
	poseR.pose.position.y = robot_pose.getOrigin().getY() - displacement*sin(yaw);
	poseR.pose.position.z = robot_pose.getOrigin().getZ();
	poseR.pose.orientation.x = robot_pose.getRotation().getX();
	poseR.pose.orientation.y = robot_pose.getRotation().getY();
	poseR.pose.orientation.z = robot_pose.getRotation().getZ();
	poseR.pose.orientation.w = robot_pose.getRotation().getW();
	local_path.poses.push_back(poseR);
	local_path_.publish(local_path);
}

void TrajectoryControlActionServer::getWheelsCmdVel(double linear_vel, double angular_vel, double robot_width, geometry_msgs::Twist& tracks_cmd)
{
	double d = robot_width/2;

	tracks_cmd.angular.x = linear_vel - (d * angular_vel);
	tracks_cmd.angular.y = linear_vel + (d * angular_vel);

	if(tracks_cmd.angular.x < -max_wheel_vel)
	{
		tracks_cmd.angular.x = -max_wheel_vel;
	}
	if(tracks_cmd.angular.x > max_wheel_vel)
	{
		tracks_cmd.angular.x = max_wheel_vel;
	}
	if(tracks_cmd.angular.y < -max_wheel_vel)
	{
		tracks_cmd.angular.y = -max_wheel_vel;
	}
	if(tracks_cmd.angular.y > max_wheel_vel)
	{
		tracks_cmd.angular.y = max_wheel_vel;
	}
}


void TrajectoryControlActionServer::executeCallback(const trajectory_control_msgs::TrajectoryControlGoalConstPtr &goal)
{
	// nifti_robot_driver_msgs::TracksStamped tracks_cmd;
	geometry_msgs::Twist tracks_cmd;

	local_path.poses.clear();
	local_path_.publish(local_path);
	global_path.poses.clear();
	global_path_.publish(global_path);

	ros::Rate rate(delay);
	double timestep = rate.expectedCycleTime().nsec/1e9;

	feedback.timestep = timestep;

	// filtering(goal->path,global_plan);
        resampleAndSmoothPath(goal->path, global_plan);

	double counter = 0;
	int index = 0;
	double duration = timestep * (global_plan.poses.size() - 1);

	geometry_msgs::Pose poseB;
	geometry_msgs::Twist velB;

	bool done = true;
	
	//ROS_INFO("counter=%f, duration=%f, index=%d, global_plan.poses.size()=%ld", (float)counter, (float)duration, index, global_plan.poses.size());

	while(ros::ok() && counter < duration && index < global_plan.poses.size())
	{
		//ROS_INFO(" ---- cycle step ----  ");
		if (as_->isPreemptRequested() || !ros::ok())
		{
			done = false;
			//ROS_INFO("%s: Preempted", action_name.c_str());
			global_plan.poses.clear();
			local_path.poses.clear();
			local_path_.publish(local_path);
			global_path.poses.clear();
			global_path_.publish(global_path);
			geometry_msgs::Twist cmdvel;
			cmdvel.linear.x = 0.0;
			cmdvel.linear.y = 0.0;
			cmdvel.angular.z = 0.0;
			// cmd_pub.publish(cmdvel);
                        //nifti_robot_driver_msgs::TracksStamped tracks_cmd;
                        //getWheelsCmdVel(0,0,robot_width,tracks_cmd);
                        tracks_vel_cmd_pub.publish(cmdvel);
                        
			as_->setPreempted();
			break;
		}
		else
		{
			//ROS_INFO("Computing commands vel....");
			//ROS_INFO("Timestep [%f]",timestep);
			//ROS_INFO("Duration [%f]",duration);
			//ROS_INFO("Counter [%f]",counter);
			buildUserDefinedTrajectory(vel_reference,global_plan.poses[index],poseB,velB);
			//ROS_INFO("BuildRefTrajectory done");
			getRobotCommands(displacement,real_robot_poseB_map,k1,k2,poseB,velB,linear_vel,angular_vel);
			//ROS_INFO("Robot commands computed");
			feedback.cmd_vel.linear.x = linear_vel;
			feedback.cmd_vel.angular.z = angular_vel;
			//getWheelsCmdVel(linear_vel,angular_vel,robot_width,tracks_cmd);
			//ROS_INFO("Robot tracks vel computed");
			//feedback.tracks_cmd.left = tracks_cmd.left;
			//feedback.tracks_cmd.right = tracks_cmd.right;
			feedback.trajectory_error.x = poseB.position.x - real_robot_poseB_map.getOrigin().getX();
			feedback.trajectory_error.y = poseB.position.y - real_robot_poseB_map.getOrigin().getY();
			as_->publishFeedback(feedback);
			tracks_cmd.linear.x = linear_vel;
			tracks_cmd.angular.z = angular_vel;
			tracks_vel_cmd_pub.publish(tracks_cmd);
			//ROS_INFO("Robot tracks vel published");
			rate.sleep();
			counter += timestep;
			index++;
			//ROS_INFO("Increments");
		}
	}
	if(done)
	{
		result.done = true;
		geometry_msgs::Twist cmdvel;
			cmdvel.linear.x = 0.0;
			cmdvel.linear.y = 0.0;
			cmdvel.angular.z = 0.0;
		//getWheelsCmdVel(0,0,robot_width,tracks_cmd);
		tracks_vel_cmd_pub.publish(cmdvel);
		rate.sleep();
		ROS_INFO("%s: Succeeded", action_name.c_str());
		global_plan.poses.clear();
		as_->setSucceeded(result);
	}
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, ros::this_node::getName());

        TrajectoryControlActionServer action_server;

	ros::spin();

	return 0;
}
