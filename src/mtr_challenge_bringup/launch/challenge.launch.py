from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    rviz_config = Path(
        get_package_share_directory("mtr_challenge_bringup")
    ) / "config" / "challenge.rviz"

    return LaunchDescription(
        [
            DeclareLaunchArgument("rviz", default_value="true"),
            Node(
                package="mtr_challenge_simulator",
                executable="simulator_node",
                name="challenge_simulator",
                output="screen",
            ),
            Node(
                package="mtr_challenge_controller",
                executable="waypoint_controller",
                name="waypoint_controller",
                output="screen",
            ),
            Node(
                package="rviz2",
                executable="rviz2",
                arguments=["-d", str(rviz_config)],
                condition=IfCondition(LaunchConfiguration("rviz")),
                output="screen",
            ),
        ]
    )

