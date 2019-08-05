// scipt to define some extra methods for my geometrical objects

#ifndef _GEOM_UTIL_
#define _GEOM_UTIL_

#include "../MyGAL/Vector2.h"
#include "../MyGAL/util.h"
using namespace mygal;

/* Returns the sign of the given value:
  * - 1 if positive
* - -1 if negative
* - 0 if null
* NB: control with tollerance since I'm working with floating point
 */
template<typename T>
int sign(const T& value){
  T eps = 10e-10; // NB: maybe you have to check this!!!!!!!!!!!!!!!!!!!!!!!!!
  if( std::fabs(value)<=eps )// std::numeric_limits<T>::epsilon()
    return 0;
  if(value > T(0))
    return 1;
  // otherwise value < T(0))
  return -1;
};

// returns true if two points are the same
template<typename T>
bool operator==(const Vector2<T>& v1, const Vector2<T>& v2){ return v1.x==v2.x && v1.y==v2.y; };

// returns true if two points are not the same
template<typename T>
bool operator!=(const Vector2<T>& v1, const Vector2<T>& v2){ return !(v1==v2); };

// computes the scalar product two vectors
template<typename T>
T dot_product(const Vector2<T>& v1, const Vector2<T>& v2){ return v1.x*v2.x+v1.y*v2.y; };

// computes the angle between two vectors (from v1 to v2 counter clockwisely)
template<typename T>
T angle_vects(const Vector2<T>& v1, const Vector2<T>& v2){
  return std::atan2( v1.getDet(v2), dot_product(v1,v2) );
};

// returns a vector rotated (counter-clockwise) by the angle theta
template<typename T>
Vector2<T> rotate(const Vector2<T>& v, const T& theta){
  T x2 = v.x*std::cos(theta)-v.y*std::sin(theta); 
  T y2 = v.x*std::sin(theta)+v.y*std::cos(theta); 
  return Vector2<T>(x2,y2);
};

#endif