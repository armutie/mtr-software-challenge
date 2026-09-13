#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "mtr_challenge_controller/controller_math.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;

class WaypointController : public rclcpp::Node
{
public:
  WaypointController()
  : Node("waypoint_controller")
  {
    command_publisher_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // The starter system is not necessarily wired correctly. Inspect the ROS graph.
    odometry_subscription_ = create_subscription<nav_msgs::msg::Odometry>(
      "/robot/odometry", 10, std::bind(&WaypointController::on_odometry, this, _1));
    waypoint_subscription_ = create_subscription<geometry_msgs::msg::PointStamped>(
      "/challenge/waypoint", rclcpp::QoS(1).transient_local(),
      std::bind(&WaypointController::on_waypoint, this, _1));
  }

private:
  void on_waypoint(const geometry_msgs::msg::PointStamped::SharedPtr message)
  {
    waypoint_x_ = message->point.x;
    waypoint_y_ = message->point.y;
    have_waypoint_ = true;
  }

  void on_odometry(const nav_msgs::msg::Odometry::SharedPtr message)
  {
    if (!have_waypoint_) {
      return;
    }

    const double x = message->pose.pose.position.x;
    const double y = message->pose.pose.position.y;
    const auto & q = message->pose.pose.orientation;
    const double yaw = std::atan2(
      2.0 * (q.w * q.z + q.x * q.y),
      1.0 - 2.0 * (q.y * q.y + q.z * q.z));

    const double dx = waypoint_x_ - x;
    const double dy = waypoint_y_ - y;
    const double distance = std::hypot(dx, dy);
    const double desired_heading = std::atan2(dy, dx);
    const double error = mtr_challenge_controller::heading_error(desired_heading, yaw);

    geometry_msgs::msg::Twist command;
    if (distance > 0.20) {
      command.linear.x = std::clamp(0.6 * distance, 0.0, 0.8);
      command.angular.z = std::clamp(1.5 * error, -1.5, 1.5);
    }
    command_publisher_->publish(command);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr command_publisher_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_subscription_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr waypoint_subscription_;
  bool have_waypoint_{false};
  double waypoint_x_{0.0};
  double waypoint_y_{0.0};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<WaypointController>());
  rclcpp::shutdown();
  return 0;
}
