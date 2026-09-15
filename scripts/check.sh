#!/usr/bin/env bash
set -eo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_dir"

source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
set -u
colcon test --packages-select mtr_software_challenge --event-handlers console_direct+
colcon test-result --verbose
