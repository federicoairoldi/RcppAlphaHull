#include <Rcpp.h>
#include "newClasses/Ball.h"
#include "newClasses/HalfPlane.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

// provides vector containings balls and halfplanes describing the alpha hull complement
template<typename T>
void complement_matrix_to_vectors(const Rcpp::NumericMatrix& complement, 
                                  std::vector<Ball<T>>& balls, std::vector<HalfPlane<T>>& halfplanes){
  // constructing balls and halfplanes that form the complement
  for(size_t i=0; i<complement.rows(); i++)
    if(complement(i,2)>0){ // r > 0 => ball
      Ball<T> b(complement(i,0), complement(i,1), complement(i,2));
      // it may happen that some balls are inserted more than one time, in those cases I just insert one
      if( std::find(balls.begin(), balls.end(), b)==balls.end() )
        balls.push_back(b);
    }
    else{
      bool side = complement(i,2) == -1 || complement(i,2) == -3? true: false; // halfplane has form with ">"
      if( complement(i,0) > -3 ) // in case r = -1 or r = -2 (non vertical halfplane)
        halfplanes.push_back(HalfPlane<T>(complement(i,1),complement(i,0),side));
      else
        halfplanes.push_back(HalfPlane<T>(complement(i,0),side));
    }
}

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

// [[Rcpp::export(".inahullRcpp")]]
Rcpp::LogicalVector inahullRcpp(const Rcpp::NumericMatrix& complement, 
                                const Rcpp::NumericVector& x, const Rcpp::NumericVector& y){
  typedef long double real;
  
  std::vector<Ball<real>> balls;
  std::vector<HalfPlane<real>> halfplanes;
  
  complement_matrix_to_vectors<real>(complement, balls, halfplanes);
  
  // spanning the points
  Rcpp::LogicalVector res(x.size());
  for(size_t i=0; i<x.size(); i++)
    res[i] = inahull_point<real>(Vector2<real>(x[i], y[i]), balls, halfplanes)? 1: 0;
  
  return res;
}