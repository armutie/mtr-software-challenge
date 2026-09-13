# Setup

## Supported environment

The initial version of this challenge targets:

- Ubuntu 24.04;
- ROS 2 Jazzy; and
- a desktop session capable of running RViz 2.

If the team gives you a prepared development environment, use it instead of installing a second copy of ROS.

## Install dependencies

Install ROS 2 Jazzy Desktop using the official ROS documentation. Then install the repository dependencies:

```bash
cd ~/mtr-software-challenge
source /opt/ros/jazzy/setup.bash
sudo apt update
sudo apt install -y python3-rosdep
sudo rosdep init  # skip this if rosdep is already initialized
rosdep update
rosdep install --from-paths src --ignore-src -r -y
```

## Build

```bash
cd ~/mtr-software-challenge
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## Launch

```bash
ros2 launch mtr_software_challenge challenge.launch.py
```

RViz should open with a blue robot, a green waypoint, and the robot's travelled path. The starter system is intentionally not fully correct.

## Verify the tests

From the repository root:

```bash
./scripts/check.sh
```

At least one controller test is expected to fail before you fix the challenge.

## Common setup problems

- **`ros2: command not found`:** source `/opt/ros/jazzy/setup.bash`.
- **Package not found after building:** source `install/setup.bash` in the current terminal.
- **RViz does not open:** confirm you installed the Desktop ROS distribution and have a graphical session.
- **Build folder contains old results:** remove `build`, `install`, and `log`, then rebuild.

If setup takes longer than 30 minutes, ask a mentor. Environment problems are not the challenge.
