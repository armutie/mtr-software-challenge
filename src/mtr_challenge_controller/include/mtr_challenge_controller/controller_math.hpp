#pragma once

namespace mtr_challenge_controller
{

inline double heading_error(const double target_heading, const double current_heading)
{
  // The starter implementation is intentionally incomplete. Angles are periodic.
  return target_heading - current_heading;
}

}  // namespace mtr_challenge_controller

