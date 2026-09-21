#pragma once

namespace mtr_software_challenge
{

inline double heading_error(double target, double current)
{

  double error = target - current;
  const double pi = 3.14159265358979323846;

  if(error<-pi){
    error+=2*pi;
  }else if ( error>pi){
    error-=2*pi;
  }

  return error;
}

}// namespace mtr_software_challenge

