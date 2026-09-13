# Getting Help

Getting stuck is normal. The goal is to practise a useful debugging process, not to struggle silently.

## Before asking

Please capture:

- the command you ran;
- the complete error or unexpected behaviour;
- what you expected instead;
- two things you already checked; and
- the relevant node/topic information.

## Progressive hints

Use only as many hints as you need.

### Hint 1 — The robot never moves

Run `rqt_graph` and select **Nodes/Topics (all)**. Look for a topic from the simulator that does not connect to the controller. Then use `ros2 node info /controller` to compare the exact subscription name.

### Hint 2 — The robot turns strangely

Print or inspect the desired heading, current heading, and their difference. Consider what happens where angles wrap from `pi` to `-pi`.

### Hint 3 — The relevant ROS commands

`ros2 node info`, `ros2 topic info`, and `ros2 topic echo` can distinguish a wiring problem from a mathematical problem.

## Asking a mentor

A useful help request looks like:

> I expected the controller to receive odometry, but its callback never runs. `/robot/odom` is publishing at 20 Hz, and `ros2 node info /waypoint_controller` shows a different subscription. I think the node is wired to the wrong topic, but I am unsure where that is configured.

Mentors should help with the next debugging step rather than provide the full solution.
