#include <cmath>

#include "gtest/gtest.h"
#include "mtr_software_challenge/controller_math.hpp"

constexpr double kPi = 3.14159265358979323846;

TEST(HeadingError, OrdinaryDifference)
{
  EXPECT_NEAR(mtr_software_challenge::heading_error(1.0, 0.25), 0.75, 1e-9);
}

TEST(HeadingError, WrapsAtPositivePi)
{
  const double target = -170.0 * kPi / 180.0;
  const double current = 170.0 * kPi / 180.0;
  EXPECT_NEAR(mtr_software_challenge::heading_error(target, current), 20.0 * kPi / 180.0, 1e-9);
}

TEST(HeadingError, WrapsAtNegativePi)
{
  const double target = 170.0 * kPi / 180.0;
  const double current = -170.0 * kPi / 180.0;
  EXPECT_NEAR(mtr_software_challenge::heading_error(target, current), -20.0 * kPi / 180.0, 1e-9);
}

