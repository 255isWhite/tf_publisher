#include <memory>
#include <iostream>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2_ros/transform_broadcaster.h"

class OdomTFPublisher : public rclcpp::Node{
    public:
        OdomTFPublisher():Node("odom_tf_publisher"){
            odom_subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
                "odometry",1,std::bind(&OdomTFPublisher::OdomCallback,this,std::placeholders::_1)
            );
            tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
            static_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        }

        void OdomCallback(const nav_msgs::msg::Odometry::SharedPtr msg){
            auto ox = msg->pose.pose.orientation.x;
            auto oy = msg->pose.pose.orientation.y;
            auto oz = msg->pose.pose.orientation.z;
            auto ow = msg->pose.pose.orientation.w;

            auto x = msg->pose.pose.position.x;
            auto y = msg->pose.pose.position.y;
            auto z = msg->pose.pose.position.z;

            geometry_msgs::msg::TransformStamped tf_msg;
            tf_msg.header.stamp = msg->header.stamp;
            tf_msg.header.frame_id = "odom";
            tf_msg.child_frame_id = "base_link";
            tf_msg.transform.translation.x = x;
            tf_msg.transform.translation.y = y;
            tf_msg.transform.translation.z = z;
            tf_msg.transform.rotation.x = ox;
            tf_msg.transform.rotation.y = oy;
            tf_msg.transform.rotation.z = oz;
            tf_msg.transform.rotation.w = ow;

            geometry_msgs::msg::TransformStamped static_msg;
            static_msg.header.stamp = msg->header.stamp;
            static_msg.header.frame_id = "base_link";
            static_msg.child_frame_id = "vehicle/chassis/gpu_lidar";
            static_msg.transform.translation.x = 0.8;
            static_msg.transform.translation.y = 0.f;
            static_msg.transform.translation.z = 0.3;
            tf2::Quaternion q;
            q.setRPY(0,0,0);
            static_msg.transform.rotation.x = q.x();
            static_msg.transform.rotation.y = q.y();
            static_msg.transform.rotation.z = q.z();
            static_msg.transform.rotation.w = q.w();

            tf_publisher_->sendTransform(tf_msg);
            static_publisher_->sendTransform(static_msg);
        }

    private:
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_;
        std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_publisher_;
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscriber_;
};

int main(int argc,char** argv){
    auto logger = rclcpp::get_logger("logger");
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<OdomTFPublisher>());
    rclcpp::shutdown();
    return 0;
}