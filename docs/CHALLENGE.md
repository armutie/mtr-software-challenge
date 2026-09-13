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

## Investigation 1 — Why is the robot stationary?

ROS 2 nodes communicate by publishing and subscribing to named topics. The intended flow is:

```text
                         waypoint
Simulator  ─────────────────────────►  Controller
    ▲                                      │
    │                                      │ velocity command
    └──────────────────────────────────────┘

Simulator  ─────── robot position ────────►  Controller
```

If a topic name differs at either end, the connection is broken even when both nodes are running.

### Step 1: draw the running system

With the challenge still running, open another terminal and run:

```bash
cd ~/mtr-software-challenge
source /opt/ros/jazzy/setup.bash
source install/setup.bash
rqt_graph
```

Select **Nodes/Topics (all)** if the topic names are hidden. Follow the arrows between the simulator and controller. You are looking for a published topic that does not reach its intended subscriber.

### Step 2: confirm the evidence

These commands answer different questions:

```bash
ros2 node list
```

Are the expected nodes alive?

```bash
ros2 topic list
```

What named communication channels exist?

```bash
ros2 topic info /robot/odom
```

How many nodes publish and subscribe to the robot-position topic? A publisher with zero subscribers is an important clue.

```bash
ros2 node info /controller
```

Which exact topic names does the controller use? Compare its subscriptions with the simulator's published topics. Names must match character for character.

### Step 3: form a conclusion

Before editing code, write down:

1. the topic carrying the robot position;
2. the name used by its publisher;
3. the name expected by the controller; and
4. why the controller cannot currently receive the message.

Find the controller's subscription in the source and repair the connection. Do not modify the simulator.

### Step 4: verify the repair

Rebuild, source the workspace, and relaunch. Then repeat:

```bash
ros2 topic info /robot/odom
```

The topic should now have one publisher and one subscriber, and the robot should begin moving. If it moves strangely, continue to Investigation 2.

## Investigation 2 — Why does the robot turn strangely?

Now inspect the desired heading, current heading, and their difference in `controller.cpp` and `controller_math.hpp`.

Useful questions:

- What units are the angles expressed in?
- Are angles ordinary numbers, or do they repeat after one full rotation?
- What should happen where an angle wraps from `pi` to `-pi`?
- What do the supplied failing tests expect?

Run the tests after each small change:

```bash
./scripts/check.sh
```

## Completion checklist

- [ ] The robot reaches the configured waypoint.
- [ ] The robot takes the short turn from its initial orientation.
- [ ] The robot stops at the target.
- [ ] `./scripts/check.sh` passes.
- [ ] No simulator or test files were changed.
- [ ] Commits explain meaningful units of work.
- [ ] The pull request explains the faults, fixes, and verification.
