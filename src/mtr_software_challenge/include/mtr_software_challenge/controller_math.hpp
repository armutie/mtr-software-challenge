#pragma once

#include <cmath>

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  double difference = target - current;

  //loop to ensure the boat is not turning the long way
  while (difference > M_PI) {
    difference -= 2.0 * M_PI;
  }
  while (difference < -M_PI) {
    difference += 2.0 * M_PI;
  }

  return difference;
}

}  // namespace mtr_software_challenge

