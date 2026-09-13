# MTR Software Challenge

A small, visual ROS 2 onboarding project for the MTR software team.

## Mission

A simulated robot has one job: reach the displayed waypoint. The starter system builds, but it does not behave correctly. Your task is to understand the ROS 2 system, diagnose the problems, repair the controller, and submit your work as a pull request.

You are finished when the robot:

- reaches the waypoint reliably;
- chooses a sensible direction of travel;
- stops within `0.20 m` of the waypoint;
- passes the supplied automated tests; and
- does not publish commands after reaching the goal.

This is an onboarding exercise, not a speed competition. Optimize for clear, reliable engineering.

## What you will practise

- building and running a ROS 2 workspace;
- reading an unfamiliar C++ codebase;
- inspecting nodes and topics;
- debugging integration and control problems;
- writing and testing a small ROS 2 change; and
- using a branch, meaningful commits, and a pull request.

## Start here

1. Read [Setup](docs/SETUP.md).
2. Read [Challenge](docs/CHALLENGE.md).
3. Create a branch named `onboarding/<your-name>`.
4. Build and launch the system.
5. Investigate before changing code.
6. Use [Getting Help](docs/GETTING_HELP.md) if you get stuck.
7. Open a pull request using the included template.

## Repository map

```text
src/
  mtr_software_challenge/    # Simulator, controller, launch, and tests
docs/                        # Setup, challenge, and help
scripts/                     # Local verification
```

Almost all participant work happens in `controller.cpp` and `controller_math.hpp`. The simulator is infrastructure and should not need modification.

## Expected time

Plan for one focused session of roughly 3–5 hours. Ask for help if setup alone takes more than 30 minutes.
