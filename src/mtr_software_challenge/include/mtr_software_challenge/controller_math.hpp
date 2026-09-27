#pragma once

#include <cmath>

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  double error = target - current;

  while (error > M_PI) {
    error -= 2.0 * M_PI;
  }

  while (error < -M_PI) {
    error += 2.0 * M_PI;
  }

  return error;
}

}  // namespace mtr_software_challenge
