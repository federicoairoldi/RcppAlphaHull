#ifndef _UTILITIES_
#define _UTILITIES_

// This header defines some general functions that are useful in my script, most of them deals with 
// vectors and Rcpp's vectors.

#include <Rcpp.h>
#include "MyGAL/FortuneAlgorithm.h"
#include "newClasses/Ball.h"
#include "newClasses/HalfPlane.h"
using namespace mygal;
using namespace Rcpp;

/* Function to translate indices: since in c++ vectors starts from 0 and in R from 1,
 * then indeces of the sites are shifted by a -1 in c++ with respect the
 * correspondent in R
 */
template<typename T1, typename T2>
std::vector<T1> add_one(const std::vector<T2>& v){
  std::vector<T1> newvett;
  for(size_t i=0; i<v.size(); i++)
    newvett.push_back(v [i]+1);
  return newvett;
};

// Returns if the point "point" is at the boundary of the box "box"
template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
  T eps(10e-5);
  if( ( std::fabs((point.x-box.left)/box.left) < eps | std::fabs((point.x-box.right)/box.right) < eps  )
        && point.y <= box.top && point.y >= box.bottom )
    return true;
  if( ( std::fabs((point.y-box.top)/box.top) < eps | std::fabs((point.y-box.bottom)/box.bottom) < eps  )
        && point.x <= box.right && point.x >= box.left )
    return true;
  return false;
}

// given a vector<T> turns it into a new vector of class Rcpp::...
template<class C, typename T>
C as(const std::vector<T>& v){
  C new_v(v.size());
  for(size_t i=0; i<v.size(); i++)
    new_v[i] = v[i];
  return new_v;
}

// provides vector containings balls and halfplanes describing the alpha hull complement
template<typename T>
void complement_matrix_to_vectors(const Rcpp::NumericMatrix& complement, 
                                  std::vector<Ball<T>>& balls, std::vector<HalfPlane<T>>& halfplanes){
  // constructing balls and halfplanes that form the complement
  for(int i=0; i<complement.rows(); i++)
    if(complement(i,2)>0){ // r > 0 => ball
      Ball<T> b(complement(i,0), complement(i,1), complement(i,2));
      // it may happen that some balls are inserted more than one time, in those cases I just insert one
      if( std::find(balls.begin(), balls.end(), b)==balls.end() )
        balls.push_back(b);
    }
    else{
      bool side = (complement(i,2) == -1 || complement(i,2) == -3)? true: false; // halfplane has form with ">"
      if( complement(i,2) > -3 )// in case r = -1 or r = -2 (non vertical halfplane)
        halfplanes.push_back(HalfPlane<T>(complement(i,1),complement(i,0),side));
      else
        halfplanes.push_back(HalfPlane<T>(complement(i,0),side));
    }
}

#endif