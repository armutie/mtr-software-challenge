#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <vector>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class ChallengeSimulator : public rclcpp::Node
{
public:
  ChallengeSimulator()
  : Node("challenge_simulator")
  {
    x_ = declare_parameter("initial_x", 0.0);
    y_ = declare_parameter("initial_y", 0.0);
    yaw_ = declare_parameter("initial_yaw", 2.96705972839);  // 170 degrees
    waypoint_x_ = declare_parameter("waypoint_x", -4.0);
    waypoint_y_ = declare_parameter("waypoint_y", -1.0);
    goal_tolerance_ = declare_parameter("goal_tolerance", 0.20);

    odometry_publisher_ = create_publisher<nav_msgs::msg::Odometry>("/robot/odom", 10);
    waypoint_publisher_ = create_publisher<geometry_msgs::msg::PointStamped>(
      "/challenge/waypoint", rclcpp::QoS(1).transient_local());
    marker_publisher_ = create_publisher<visualization_msgs::msg::MarkerArray>(
      "/challenge/markers", 10);
    goal_publisher_ = create_publisher<std_msgs::msg::Bool>(
      "/challenge/goal_reached", rclcpp::QoS(1).transient_local());
    command_subscription_ = create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 10, std::bind(&ChallengeSimulator::on_command, this, _1));

    last_update_ = now();
    last_command_ = now();
    timer_ = create_wall_timer(50ms, std::bind(&ChallengeSimulator::update, this));

    publish_waypoint();
    RCLCPP_INFO(get_logger(), "Challenge ready: reach waypoint (%.2f, %.2f)", waypoint_x_, waypoint_y_);
  }

private:
  void on_command(const geometry_msgs::msg::Twist::SharedPtr message)
  {
    linear_velocity_ = std::clamp(message->linear.x, 0.0, 0.8);
    angular_velocity_ = std::clamp(message->angular.z, -1.5, 1.5);
    last_command_ = now();
  }

  void update()
  {
    const auto current_time = now();
    const double dt = std::min((current_time - last_update_).seconds(), 0.1);
    last_update_ = current_time;

    if ((current_time - last_command_).seconds() > 0.5) {
      linear_velocity_ = 0.0;
      angular_velocity_ = 0.0;
    }

    yaw_ = normalize_angle(yaw_ + angular_velocity_ * dt);
    x_ += linear_velocity_ * std::cos(yaw_) * dt;
    y_ += linear_velocity_ * std::sin(yaw_) * dt;
    path_.push_back(make_point(x_, y_));
    if (path_.size() > 1500) {
      path_.erase(path_.begin());
    }

    const double distance = std::hypot(waypoint_x_ - x_, waypoint_y_ - y_);
    const bool reached = distance <= goal_tolerance_;
    if (reached && !goal_reached_) {
      goal_reached_ = true;
      RCLCPP_INFO(get_logger(), "GOAL REACHED");
    }

    publish_odometry(current_time);
    publish_visualization(current_time);
    publish_goal(reached);
  }

  static double normalize_angle(double angle)
  {
    return std::atan2(std::sin(angle), std::cos(angle));
  }

  static geometry_msgs::msg::Point make_point(const double x, const double y)
  {
    geometry_msgs::msg::Point point;
    point.x = x;
    point.y = y;
    return point;
  }

  geometry_msgs::msg::Quaternion orientation() const
  {
    geometry_msgs::msg::Quaternion quaternion;
    quaternion.z = std::sin(yaw_ / 2.0);
    quaternion.w = std::cos(yaw_ / 2.0);
    return quaternion;
  }

  void publish_waypoint()
  {
    geometry_msgs::msg::PointStamped waypoint;
    waypoint.header.frame_id = "map";
    waypoint.header.stamp = now();
    waypoint.point = make_point(waypoint_x_, waypoint_y_);
    waypoint_publisher_->publish(waypoint);
  }

  void publish_odometry(const rclcpp::Time & stamp)
  {
    nav_msgs::msg::Odometry odometry;
    odometry.header.frame_id = "map";
    odometry.child_frame_id = "base_link";
    odometry.header.stamp = stamp;
    odometry.pose.pose.position = make_point(x_, y_);
    odometry.pose.pose.orientation = orientation();
    odometry.twist.twist.linear.x = linear_velocity_;
    odometry.twist.twist.angular.z = angular_velocity_;
    odometry_publisher_->publish(odometry);
  }

  void publish_goal(const bool reached)
  {
    std_msgs::msg::Bool status;
    status.data = reached;
    goal_publisher_->publish(status);
  }

  visualization_msgs::msg::Marker base_marker(
    const int id, const int type, const rclcpp::Time & stamp) const
  {
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "map";
    marker.header.stamp = stamp;
    marker.ns = "mtr_challenge";
    marker.id = id;
    marker.type = type;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.orientation.w = 1.0;
    return marker;
  }

  void publish_visualization(const rclcpp::Time & stamp)
  {
    visualization_msgs::msg::MarkerArray markers;

    auto robot = base_marker(0, visualization_msgs::msg::Marker::ARROW, stamp);
    robot.pose.position = make_point(x_, y_);
    robot.pose.orientation = orientation();
    robot.scale.x = 0.65;
    robot.scale.y = 0.35;
    robot.scale.z = 0.25;
    robot.color.r = 0.10F;
    robot.color.g = 0.45F;
    robot.color.b = 1.00F;
    robot.color.a = 1.00F;
    markers.markers.push_back(robot);

    auto waypoint = base_marker(1, visualization_msgs::msg::Marker::SPHERE, stamp);
    waypoint.pose.position = make_point(waypoint_x_, waypoint_y_);
    waypoint.scale.x = goal_tolerance_ * 2.0;
    waypoint.scale.y = goal_tolerance_ * 2.0;
    waypoint.scale.z = 0.25;
    waypoint.color.r = goal_reached_ ? 1.00F : 0.10F;
    waypoint.color.g = goal_reached_ ? 0.80F : 0.90F;
    waypoint.color.b = 0.10F;
    waypoint.color.a = 1.00F;
    markers.markers.push_back(waypoint);

    auto path = base_marker(2, visualization_msgs::msg::Marker::LINE_STRIP, stamp);
    path.scale.x = 0.04;
    path.color.r = 0.25F;
    path.color.g = 0.65F;
    path.color.b = 1.00F;
    path.color.a = 0.80F;
    path.points = path_;
    markers.markers.push_back(path);

    marker_publisher_->publish(markers);
  }

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr waypoint_publisher_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_publisher_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr goal_publisher_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr command_subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time last_update_;
  rclcpp::Time last_command_;
  std::vector<geometry_msgs::msg::Point> path_;
  double x_{0.0};
  double y_{0.0};
  double yaw_{0.0};
  double waypoint_x_{0.0};
  double waypoint_y_{0.0};
  double goal_tolerance_{0.20};
  double linear_velocity_{0.0};
  double angular_velocity_{0.0};
  bool goal_reached_{false};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ChallengeSimulator>());
  rclcpp::shutdown();
  return 0;
}
