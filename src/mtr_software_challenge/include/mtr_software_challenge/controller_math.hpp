#pragma once

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  // TODO: Angles wrap around at -pi and pi.
  return target - current;
}

}  // namespace mtr_software_challenge

