#include <Rcpp.h>
#include "newClasses/Ball.h"
#include "newClasses/HalfPlane.h"
#include "MyGAL/Vector2.h"
#include "utilities.h"
using namespace Rcpp;

// checks if the given point falls in the alpha-hull whose complmenet is described by the two vectors
// balls and halfplanes
template<typename T>
bool inahull_point(const Vector2<T>& point, 
                   const std::vector<Ball<T>>& balls, const std::vector<HalfPlane<T>>& halfplanes){
  // checking if the point is in one of the halfplanes
  for(size_t i=0; i<halfplanes.size(); i++)
    if(halfplanes[i].isIn(point))
      return false;
  
  // checking if the point is in one of the balls
  for(size_t i=0; i<balls.size(); i++)
    if(balls[i].isIn(point))
      return false;
  
  // if the point isn't in a ball or in an halfplane then it's in the alpha hull
  return true;
}

// This function evaluates whether or not the points (x and y coordinates) fall in the alpha hull denoted
// by the the given complement matrix
// [[Rcpp::export(".inahullRcpp")]]
Rcpp::LogicalVector inahullRcpp(const Rcpp::NumericMatrix& complement, 
                                const Rcpp::NumericVector& x, const Rcpp::NumericVector& y){
  typedef long double real;
  
  std::vector<Ball<real>> balls;
  std::vector<HalfPlane<real>> halfplanes;
  
  complement_matrix_to_vectors<real>(complement, balls, halfplanes);
  
  // spanning the points
  Rcpp::LogicalVector res(x.size());
  for(int i=0; i<x.size(); i++)
    res[i] = inahull_point<real>(Vector2<real>(x[i], y[i]), balls, halfplanes)? 1: 0;
  
  return res;
}