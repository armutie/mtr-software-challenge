# MTR Software Challenge

Welcome to the MTR Software Challenge. This project is a simplified simulation of a real autonomous-boat control loop. The blue arrow in RViz represents the boat, and the green marker represents a person in distress. Your task is to find and fix two controller bugs so the boat reaches five waypoints reliably.

Work on your own branch and push that branch when you are finished.

## Repository access

Every participant must be added as a collaborator before pushing to this repository. If you have not already been added, send us your GitHub username and email address when you are ready to submit. This is especially important if your GitHub account is not associated with a University of Waterloo (`@uwaterloo.ca`) address.

If you cannot push your branch, send us the files containing your code changes instead.

## Requirements

- Ubuntu 24.04, either installed directly or through [WSL 2](https://learn.microsoft.com/windows/wsl/install)
- [ROS 2 Jazzy Desktop](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)
- A desktop session capable of running RViz 2

Unless a step specifically says Windows PowerShell, run each command in an Ubuntu terminal.

## Download the project

Install Git and the required ROS tools:

```bash
sudo apt update
sudo apt install -y git python3-rosdep python3-colcon-common-extensions
```

Clone the repository and enter the project directory:

```bash
cd ~
git clone https://github.com/armutie/mtr-software-challenge.git
cd mtr-software-challenge
```

Create a branch for your work. Replace `firstname` with your own first name:

```bash
git switch -c onboarding-firstname
```

## Install the dependencies

```bash
source /opt/ros/jazzy/setup.bash
sudo rosdep init
rosdep update
rosdep install --from-paths src --ignore-src -r -y
```

If `sudo rosdep init` reports that rosdep has already been initialized, continue to the next command.

## Build and launch

```bash
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
ros2 launch mtr_software_challenge challenge.launch.py
```

RViz should open with a blue boat, a green waypoint, and a grid. The boat will not initially move; this is the starting point of the challenge. After each waypoint is reached, the green marker moves to the next location. Press `Ctrl+C` in the terminal to stop the simulation.

After changing C++ code, stop the simulation, rebuild, source the workspace, and launch it again:

```bash
colcon build --symlink-install
source install/setup.bash
ros2 launch mtr_software_challenge challenge.launch.py
```

## The challenge

The simulator publishes the boat's current position and the active waypoint. The controller should read that information, decide how quickly the boat should move and turn, and publish a velocity command back to the simulator. The simulator presents five waypoints, one at a time, and reports the travel time for each one.

There are two intentional problems:

1. The boat does not move when the starter project is launched.
2. After the first problem is repaired, the boat may turn the long way around.

Make functional changes only in:

```text
src/mtr_software_challenge/src/controller.cpp
src/mtr_software_challenge/include/mtr_software_challenge/controller_math.hpp
```

Treat `simulator.cpp` as working infrastructure. Do not change the waypoint, initial pose, simulator, or supplied tests.

### General hints

- Compare the running system with the intended publisher and subscriber flow.
- `rqt_graph` and the ROS 2 command-line tools can help you inspect connections between nodes.
- Topic names and message types must match exactly.
- Angles are measured in radians and wrap around at `-pi` and `+pi`.
- The boat should choose the shortest direction of rotation.

## Optional challenge: improve efficiency

Once the two required problems are fixed, watch the complete route in RViz. The boat may move forward while it is still facing away from a waypoint, producing a wide and inefficient path.

If you finish early, improve the controller so the boat completes all five waypoints more efficiently. Consider how the heading error should affect forward speed. There is no required formula, but the boat must remain reliable, choose the shortest direction of rotation, and stop at every waypoint. Do not change the simulator or waypoint locations.

The simulator prints the travel time for each waypoint and the average after all five are reached. This extension is optional and is not required to complete the challenge.

## Run the tests

From the repository root, run:

```bash
./scripts/check.sh
```

In the untouched starter code, one heading test passes and two heading-wrapping tests fail. After completing the challenge, all three tests should pass.

## Completion and submission

You are finished when:

- the boat reaches all five waypoints;
- it chooses the shortest reasonable direction when turning;
- it stops within `0.20 m` of each waypoint;
- all supplied tests pass; and
- your functional changes are limited to the two controller files listed above.

Review and commit your work:

```bash
git status
git diff
git add src/mtr_software_challenge/src/controller.cpp
git add src/mtr_software_challenge/include/mtr_software_challenge/controller_math.hpp
git commit -m "Fix boat controller"
```

Push your branch, replacing the example name with your actual branch name:

```bash
git push -u origin onboarding-firstname
```

Once it has been pushed, send us the name of your branch.
