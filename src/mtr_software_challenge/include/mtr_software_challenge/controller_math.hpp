#pragma once
#include <cmath>

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  // TODO: Angles wrap around at -pi and pi.
  double dAngle = target - current;
  if (dAngle > M_PI) {
    return dAngle - 2 * M_PI;
  }
  else if(dAngle < -M_PI) {
    return dAngle + 2 * M_PI;
  }
  return dAngle;
}

}  // namespace mtr_software_challenge

