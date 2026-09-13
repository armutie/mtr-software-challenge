# Challenge

## Scenario

The simulator publishes the robot's motion and the waypoint. The controller should observe both, compute a command, and drive the robot to the target.

The starter code compiles, but the running system is not trustworthy. There are two intentional problems in the controller code. Both represent mistakes that occur in real robotic systems.

## Rules

- Treat `simulator.cpp` as working infrastructure.
- Make your functional changes in `controller.cpp` and `controller_math.hpp`.
- Do not change the waypoint or initial pose to avoid a problem.
- Do not weaken or remove tests.
- Use ROS 2 inspection tools before editing code.
- Keep the solution understandable to another team member.

## Required behaviour

The controller must:

- consume robot odometry and the published waypoint;
- turn toward the waypoint using the shortest angular direction;
- keep linear speed in `[0.0, 0.8] m/s`;
- keep angular speed in `[-1.5, 1.5] rad/s`;
- slow down as it approaches the waypoint;
- publish a zero command within `0.20 m` of the waypoint; and
- pass all supplied tests.

## Suggested investigation

Do not assume a silent robot means the control equation is wrong. First build a picture of the running system:

```bash
ros2 node list
ros2 topic list
ros2 topic info /robot/odom
ros2 topic echo /challenge/waypoint --once
ros2 topic hz /robot/odom
```

Useful questions:

- Which nodes are running?
- Which topics does each node publish and subscribe to?
- Are the topic names and message types compatible?
- Is the controller receiving callbacks?
- What are the units of each value?
- What happens to an angle near `-pi` and `pi`?

## Completion checklist

- [ ] The robot reaches the configured waypoint.
- [ ] The robot takes the short turn from its initial orientation.
- [ ] The robot stops at the target.
- [ ] `./scripts/check.sh` passes.
- [ ] No simulator or test files were changed.
- [ ] Commits explain meaningful units of work.
- [ ] The pull request explains the faults, fixes, and verification.
