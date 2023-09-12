/*
 * StigmergyPlanner.h
 *
 *  Created on: Jun 27, 2013
 *      Author: alcor
 */

#ifndef STIGMERGYPLANNER_H_
#define STIGMERGYPLANNER_H_

#include <interactive_markers/interactive_marker_server.h>
#include <interactive_markers/menu_handler.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>

#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <math.h>

#include <visualization_msgs/Marker.h>
#include <actionlib/client/simple_action_client.h>
#include <trajectory_control_msgs/TrajectoryControlAction.h>
#include <sensor_msgs/PointCloud2.h>

#include "pcl_ros/point_cloud.h"
#include "pcl/point_types.h"

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

typedef actionlib::SimpleActionClient<trajectory_control_msgs::TrajectoryControlAction> TrajectoryControlClient;

using namespace visualization_msgs;

class StigmergyPlanner
{
    
private:
    ros::NodeHandle n_;
protected:
    std::string global_frame_id;
    std::string robot_frame_id;
    std::string trajectory_control_action_server_name;
    std::string interactive_marker_server_name;
    std::string pcd_map_topic_name;
    
    std::string stigmergy_path_topic_name;
    // std::string final_stigmergy_path_topic_name;
    std::string overlaid_path_topic_name;
    std::string interpolated_path_topic_name;
    
    std::string target_topic_name;
    
    ros::NodeHandle node;
    
public:

	boost::shared_ptr<interactive_markers::InteractiveMarkerServer> server;
	interactive_markers::MenuHandler menu_handler;

	TrajectoryControlClient* ac;

	StigmergyPlanner();
	~StigmergyPlanner();

	Marker makeBox(InteractiveMarker&);
	InteractiveMarkerControl& makeBoxControl(InteractiveMarker&);
	void makeViewFacingMarker(tf::StampedTransform);
	void processFeedback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr&);

	bool getRobotPose(tf::StampedTransform&);

	void reset();

	bool starting_pose_ready;
	geometry_msgs::PoseStamped starting_pose;

	bool drawn_path;
	bool finished;
	nav_msgs::Path path3D;
	nav_msgs::Path path_overlaid;
	nav_msgs::Path path_interp;
	ros::Publisher path_pub;
	// ros::Publisher final_path_pub;
	
	InteractiveMarker int_marker;

	visualization_msgs::Marker target;
	ros::Publisher target_pub;

	tf::TransformListener tf_;
	tf::StampedTransform robot_pose;

	void pcl_callback(const sensor_msgs::PointCloud2ConstPtr&);
	ros::Subscriber pcl_sub;
	pcl::PointCloud<pcl::PointXYZ> point_cloud_in;
	pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_in_ptr;
	void overlay(pcl::PointCloud<pcl::PointXYZ>::Ptr,nav_msgs::Path,nav_msgs::Path&);
	ros::Publisher path_ovelaid_pub;
	void interpolate(nav_msgs::Path,nav_msgs::Path&);
	ros::Publisher path_interp_pub;
};


#endif /* STIGMERGYPLANNER_H_ */
