#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

using namespace std::chrono_literals;

class Simulator : public rclcpp::Node
{
public:
  Simulator() : Node("simulator"), last_update_(now())
  {
    odometry_ = create_publisher<nav_msgs::msg::Odometry>("/robot/odom", 10);
    waypoint_ = create_publisher<geometry_msgs::msg::PointStamped>("/challenge/waypoint", 10);
    markers_ = create_publisher<visualization_msgs::msg::MarkerArray>("/challenge/markers", 10);
    commands_ = create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 10,
      [this](geometry_msgs::msg::Twist::SharedPtr message) { command_ = *message; });
    timer_ = create_wall_timer(50ms, [this]() { update(); });
  }

private:
  static geometry_msgs::msg::Point point(double x, double y)
  {
    geometry_msgs::msg::Point result;
    result.x = x;
    result.y = y;
    return result;
  }

  geometry_msgs::msg::Quaternion orientation() const
  {
    geometry_msgs::msg::Quaternion result;
    result.z = std::sin(yaw_ / 2.0);
    result.w = std::cos(yaw_ / 2.0);
    return result;
  }

  visualization_msgs::msg::Marker marker(int id, int type) const
  {
    visualization_msgs::msg::Marker result;
    result.header.frame_id = "map";
    result.header.stamp = now();
    result.ns = "challenge";
    result.id = id;
    result.type = type;
    result.action = visualization_msgs::msg::Marker::ADD;
    result.pose.orientation.w = 1.0;
    return result;
  }

  void update()
  {
    const auto time = now();
    const double dt = (time - last_update_).seconds();
    last_update_ = time;

    yaw_ = std::atan2(
      std::sin(yaw_ + command_.angular.z * dt),
      std::cos(yaw_ + command_.angular.z * dt));
    x_ += command_.linear.x * std::cos(yaw_) * dt;
    y_ += command_.linear.x * std::sin(yaw_) * dt;
    path_.push_back(point(x_, y_));

    nav_msgs::msg::Odometry odometry;
    odometry.header.frame_id = "map";
    odometry.header.stamp = time;
    odometry.pose.pose.position = point(x_, y_);
    odometry.pose.pose.orientation = orientation();
    odometry_->publish(odometry);

    geometry_msgs::msg::PointStamped waypoint;
    waypoint.header.frame_id = "map";
    waypoint.header.stamp = time;
    waypoint.point = point(goal_x_, goal_y_);
    waypoint_->publish(waypoint);

    publish_markers();

    if (!reached_ && std::hypot(goal_x_ - x_, goal_y_ - y_) <= 0.20) {
      reached_ = true;
      RCLCPP_INFO(get_logger(), "GOAL REACHED");
    }
  }

  void publish_markers()
  {
    visualization_msgs::msg::MarkerArray all;

    auto robot = marker(0, visualization_msgs::msg::Marker::ARROW);
    robot.pose.position = point(x_, y_);
    robot.pose.orientation = orientation();
    robot.scale.x = 0.65;
    robot.scale.y = 0.35;
    robot.scale.z = 0.25;
    robot.color.b = 1.0F;
    robot.color.a = 1.0F;
    all.markers.push_back(robot);

    auto goal = marker(1, visualization_msgs::msg::Marker::SPHERE);
    goal.pose.position = point(goal_x_, goal_y_);
    goal.scale.x = 0.4;
    goal.scale.y = 0.4;
    goal.scale.z = 0.25;
    goal.color.g = 1.0F;
    goal.color.a = 1.0F;
    all.markers.push_back(goal);

    auto path = marker(2, visualization_msgs::msg::Marker::LINE_STRIP);
    path.scale.x = 0.04;
    path.color.b = 1.0F;
    path.color.g = 0.6F;
    path.color.a = 0.8F;
    path.points = path_;
    all.markers.push_back(path);

    markers_->publish(all);
  }

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr waypoint_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr markers_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr commands_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time last_update_;
  geometry_msgs::msg::Twist command_;
  std::vector<geometry_msgs::msg::Point> path_;
  double x_{0.0};
  double y_{0.0};
  double yaw_{2.96705972839};  // 170 degrees
  const double goal_x_{-4.0};
  const double goal_y_{-1.0};
  bool reached_{false};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Simulator>());
  rclcpp::shutdown();
}

