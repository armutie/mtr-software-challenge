#include <cmath>

#include "gtest/gtest.h"
#include "mtr_challenge_controller/controller_math.hpp"

namespace
{
constexpr double kPi = 3.14159265358979323846;
}

TEST(ControllerMath, OrdinaryHeadingDifference)
{
  EXPECT_NEAR(mtr_challenge_controller::heading_error(1.0, 0.25), 0.75, 1e-9);
}

TEST(ControllerMath, WrapsAcrossPositivePi)
{
  const double target = -170.0 * kPi / 180.0;
  const double current = 170.0 * kPi / 180.0;
  EXPECT_NEAR(mtr_challenge_controller::heading_error(target, current), 20.0 * kPi / 180.0, 1e-9);
}

TEST(ControllerMath, WrapsAcrossNegativePi)
{
  const double target = 170.0 * kPi / 180.0;
  const double current = -170.0 * kPi / 180.0;
  EXPECT_NEAR(mtr_challenge_controller::heading_error(target, current), -20.0 * kPi / 180.0, 1e-9);
}

