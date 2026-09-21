#pragma once

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{

double pi = 3.14159265359;
double diff = target - current;

if (diff > pi)
{
 diff = diff - 2 * pi;
}
else if (diff < -pi)
{
 diff = diff + 2 * pi;

}
  // TODO: Angles wrap around at -pi and pi.
  return diff;
}

}  // namespace mtr_software_challenge
