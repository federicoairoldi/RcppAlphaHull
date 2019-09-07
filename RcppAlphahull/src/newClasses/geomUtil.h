// scipt to define some extra methods for my geometrical objects

#ifndef _GEOM_UTIL_
#define _GEOM_UTIL_

#include <list>
#include "../MyGAL/Vector2.h"
#include "../MyGAL/util.h"
using namespace mygal;

template<typename T> class Ball;
template<typename T> class CircArc;

/* Returns the sign of the given value:
  * - 1 if positive
  * - -1 if negative
  * - 0 if null
  * NB: control with tollerance since I'm working with floating point
   */
template<typename T>
int sign(const T& value){
  if( std::fabs(value) <= std::numeric_limits<T>::epsilon() )  //  EPSILON<T>
    return 0;
  if(value > T(0))
    return 1;
  // otherwise value < T(0))
  return -1;
};

// Given a vector2, returns its normalization (returns the vector itself if it is null)
template<typename T>
Vector2<T> normalize_vect(const Vector2<T>& v){ 
  if(v.getNorm()>0)
    return 1/v.getNorm()*v;
  return v;
};

// Returns true if two points are the same
template<typename T>
bool operator==(const Vector2<T>& v1, const Vector2<T>& v2){ return v1.x==v2.x && v1.y==v2.y; };

// Returns true if two points are different
template<typename T>
bool operator!=(const Vector2<T>& v1, const Vector2<T>& v2){ return !(v1==v2); };

// Computes the scalar product two vectors
template<typename T>
T dot_product(const Vector2<T>& v1, const Vector2<T>& v2){ return v1.x*v2.x+v1.y*v2.y; };

// Computes the angle between two vectors (from v1 to v2 counter clockwisely)
template<typename T>
T angle_vects(const Vector2<T>& v1, const Vector2<T>& v2){
  T angle = std::atan2( v1.getDet(v2), dot_product(v1,v2) );
  if(angle < 0)
    angle+=2*M_PI;
  return angle;
};

// Rturns a vectore rotated (counter-clockwise) by the angle theta
template<typename T>
Vector2<T> rotate(const Vector2<T>& v, const T& theta){
  T x2 = v.x*std::cos(theta)-v.y*std::sin(theta); 
  T y2 = v.x*std::sin(theta)+v.y*std::cos(theta); 
  return Vector2<T>(x2,y2);
};

// Given a vector of arcs, removes from each one the parts in common with the provided ball returning
// a vector containing the new arc parts
template<typename T>
std::vector<CircArc<T>> collective_removeBall(const std::vector<CircArc<T>>& arcs, const Ball<T>& b){
  std::vector<CircArc<T>> res;
  
  for(typename std::vector<CircArc<T>>::const_iterator it = arcs.cbegin(); it!=arcs.cend(); it++){
    std::vector<CircArc<T>> tmp = it->removeBall(b); // subtracting the ball from the current element
    res.insert(res.end(), tmp.begin(), tmp.end()); // adding the subtracted arcs to the result
  }
  
  return res;
}

// Given some balls returns the arcs that form the boundary of the union by removing those arc parts 
// that are in the interior
template<typename T>
std::vector<CircArc<T>> union_boundary(const std::vector<Ball<T>>& balls){
  std::vector<CircArc<T>> res;
  
  // for each ball I create the respective arc and then remove the portions inside the other balls
  for(typename std::vector<Ball<T>>::const_iterator it1=balls.cbegin(); it1!=balls.cend(); it1++){
    CircArc<T> arc(*it1); // in principle the whole ball boundary has to be included
    std::vector<CircArc<T>> tmp{arc}; // in this variable I progressively save the results I get by
                                      // removing the different balls
                                      
    typename std::vector<Ball<T>>::const_iterator it2=balls.cbegin();
    while(it2!=balls.cend() && !tmp.empty()){
      if( it1!=it2 )
        tmp = collective_removeBall(tmp, *it2);
      it2++;
    }
    res.insert(res.end(), tmp.begin(), tmp.end());
  }
  
  return res;
}

#endif