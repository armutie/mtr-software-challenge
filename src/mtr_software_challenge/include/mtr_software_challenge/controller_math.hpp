#pragma once
#include <cmath>

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  // TODO: Angles wrap around at -pi and pi.
  double error = target - current;

  //If target and current angles were around the values of |pi|, then the difference between the two would be close to 360
  //This means the boat would travel in almost an entire circle first (since circle is 360 degrees) before moving along the path -> NOT the shortest distance possible
  //Since 360 is 2pi, subtracting or adding 2pi from the difference would allow the shortest distance possible

  if(error > M_PI){
    error -= M_PI + M_PI;
  }
  else if(error < -M_PI){
    error += M_PI + M_PI;
  }

  return error;
}

}  // namespace mtr_software_challenge

