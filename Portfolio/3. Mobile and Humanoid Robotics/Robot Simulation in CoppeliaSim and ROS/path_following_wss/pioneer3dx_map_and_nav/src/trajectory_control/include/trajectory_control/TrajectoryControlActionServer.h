/*
 * TrajectoryControlActionServer.h
 *
 *  Created on: Jan 13, 2014
 *      Author: alcor
 */

#ifndef TRAJECTORYCONTROLACTIONSERVER_H_
#define TRAJECTORYCONTROLACTIONSERVER_H_

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <trajectory_control_msgs/TrajectoryControlAction.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
//#include <nifti_robot_driver_msgs/Tracks.h>
//#include <nifti_robot_driver_msgs/TracksStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/Imu.h>
#include <actionlib/client/simple_action_client.h>

#define FEQUAL(a,b,tol) ( ( fabs(a-b) < (tol) ) ? (1) : (0) ) 


template<typename T>
T getParam(ros::NodeHandle& n, const std::string& name, const T& defaultValue)
{
	T v;
	if (n.getParam(name, v))
	{
		ROS_INFO_STREAM("Found parameter: " << name << ", value: " << v);
		return v;
	}
	else
		ROS_WARN_STREAM("Cannot find value for parameter: " << name << ", assigning default: " << defaultValue);
	return defaultValue;
}


class TrajectoryControlActionServer
{
    
private:
    
    ros::NodeHandle n_;
    
protected:
    
	ros::NodeHandle node;
        
        std::string odom_frame_id;
	std::string global_frame_id;
	std::string robot_frame_id;
        
       	std::string action_name;
        std::string trajectory_control_action_server_name;
                
        actionlib::SimpleActionServer<trajectory_control_msgs::TrajectoryControlAction>* as_;
	actionlib::SimpleActionClient<trajectory_control_msgs::TrajectoryControlAction>* ac_;

	trajectory_control_msgs::TrajectoryControlFeedback feedback;
	trajectory_control_msgs::TrajectoryControlResult result;

	double displacement;
	double vel_reference;
	// std::vector<double> tip_over_axes_coeff;

	tf::TransformListener tf_;

	std::string imu_odom_topic;
	ros::Subscriber imu_odom_sub;

	std::string tracks_vel_cmd_topic;
	ros::Publisher tracks_vel_cmd_pub;

	tf::StampedTransform real_robot_pose_map;
	tf::StampedTransform real_robot_poseB_map;
	tf::StampedTransform current_real_robot_pose_odom;
	tf::StampedTransform from_odom_to_map;

	double linear_vel;
	double angular_vel;
	double robot_width;
	double k1;
	double k2;
	double delay;
        double max_wheel_vel;

	std::string global_path_topic;
	ros::Publisher global_path_;

	std::string local_path_topic;
	ros::Publisher local_path_;

	nav_msgs::Path global_path;
	nav_msgs::Path local_path;
	nav_msgs::Path global_plan;


	std::string robot_path_topic;


 ros::Subscriber nav_path_sub;


public:
        TrajectoryControlActionServer();
	~TrajectoryControlActionServer();
	void odomMsgToStampedTransform(nav_msgs::Odometry,tf::StampedTransform&);
	void realRobotPoseB(double,tf::StampedTransform,tf::StampedTransform&);
	bool getRobotPose(tf::StampedTransform&,tf::StampedTransform&,tf::StampedTransform&);
	void filtering(nav_msgs::Path,nav_msgs::Path&);
	void buildUserDefinedTrajectory(double,geometry_msgs::PoseStamped,geometry_msgs::Pose&,geometry_msgs::Twist&);
	void getRobotCommands(double, tf::StampedTransform, double, double, geometry_msgs::Pose, geometry_msgs::Twist, double&, double&);
	void getWheelsCmdVel(double,double,double,geometry_msgs::Twist&);
	void imuOdomCallback(const nav_msgs::OdometryConstPtr&);
	void executeCallback(const trajectory_control_msgs::TrajectoryControlGoalConstPtr&);
	

	void pathCallBack(const nav_msgs::PathConstPtr&);
        void resampleAndSmoothPath(const nav_msgs::Path&, nav_msgs::Path&);

};


#endif /* TRAJECTORYCONTROLACTIONSERVER_H_ */
