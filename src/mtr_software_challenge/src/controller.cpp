#include <algorithm>
#include <cmath>
#include <memory>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "mtr_software_challenge/controller_math.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"

class Controller : public rclcpp::Node
{
public:
  Controller() : Node("controller")
  {
    commands_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    odometry_ = create_subscription<nav_msgs::msg::Odometry>(
      "/robot/odometry", 10,
      [this](nav_msgs::msg::Odometry::SharedPtr message) { drive(*message); });

    waypoint_ = create_subscription<geometry_msgs::msg::PointStamped>(
      "/challenge/waypoint", 10,
      [this](geometry_msgs::msg::PointStamped::SharedPtr message) {
        goal_x_ = message->point.x;
        goal_y_ = message->point.y;
        have_goal_ = true;
      });
  }

private:
  void drive(const nav_msgs::msg::Odometry & odometry)
  {
    if (!have_goal_) {
      return;
    }

    const auto & position = odometry.pose.pose.position;
    const auto & q = odometry.pose.pose.orientation;
    const double yaw = std::atan2(2.0 * q.w * q.z, 1.0 - 2.0 * q.z * q.z);
    const double dx = goal_x_ - position.x;
    const double dy = goal_y_ - position.y;
    const double distance = std::hypot(dx, dy);
    const double desired_yaw = std::atan2(dy, dx);

    geometry_msgs::msg::Twist command;
    if (distance > 0.20) {
      command.linear.x = std::clamp(0.6 * distance, 0.0, 0.8);
      command.angular.z = std::clamp(
        1.5 * mtr_software_challenge::heading_error(desired_yaw, yaw), -1.5, 1.5);
    }
    commands_->publish(command);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr commands_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr waypoint_;
  bool have_goal_{false};
  double goal_x_{0.0};
  double goal_y_{0.0};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Controller>());
  rclcpp::shutdown();
}

