from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = Path(get_package_share_directory("mtr_software_challenge")) / "config/challenge.rviz"
    return LaunchDescription([
        Node(
            package="tf2_ros",
            executable="static_transform_publisher",
            arguments=[
                "--frame-id", "map",
                "--child-frame-id", "visualization_anchor",
            ],
        ),
        Node(package="mtr_software_challenge", executable="simulator", output="screen"),
        Node(package="mtr_software_challenge", executable="controller", output="screen"),
        Node(package="rviz2", executable="rviz2", arguments=["-d", str(config)]),
    ])
