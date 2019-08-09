#ifndef _UTILITIES_
#define _UTILITIES_

// This header defines some general functions that are useful in my script, most of them deals with 
// vectors and Rcpp's vectors.

#include <Rcpp.h>
#include "MyGAL/FortuneAlgorithm.h"
using namespace mygal;
using namespace Rcpp;

/* Function to translate indices: since in c++ vectors starts from 0 and in R from 1,
 * then indeces of the sites are shifted by a -1 in c++ with respect the
 * correspondent in R
 */
template<typename T>
std::vector<T> add_one(const std::vector<T>& v){
  std::vector<T> newvett = v;
  for(size_t i=0; i<newvett.size(); i++)
    newvett[i]+=1;
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

#endif