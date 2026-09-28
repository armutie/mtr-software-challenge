#pragma once
#include <cmath>
namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  // TODO: Angles wrap around at -pi and pi.
  double diff = target - current;
  // Makes it so that itll turn the short way around each time
  if (diff > M_PI){
    diff -= 2.0*M_PI;
  } else if (diff < -M_PI){
    diff += 2.0*M_PI;
  }
  
  return diff;
}

}  // namespace mtr_software_challenge

