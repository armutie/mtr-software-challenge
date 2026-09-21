#pragma once

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{
  const double pi = 3.141592653589793;
  double error = target - current;

  while (error > pi) {
    error -= 2.0 * pi;
  }

  while (error < -pi) {
    error += 2.0 * pi;
  }

  return error;
}

}  // namespace mtr_software_challenge
