#ifndef _BALL_
#define _BALL_

#include <ostream>
#include "../MyGAL/Vector2.h"
using namespace mygal;

template<typename T>
class Ball{
private:
    Vector2<T> c; // center of the ball
    T r; // radius of the ball

  public:
    Ball(): c(0,0), r(0) {}; // default creates a degenerate ball centered in O with 0 radius
    Ball(const Vector2<T>& c, const T& r): c(c), r(r) {};
    Ball(const T& xc, const T& yc, const T& r): c(xc,yc), r(r) {};

    Vector2<T> center() const { return c; };
    T radius() const { return r; };
    bool isInside(const Vector2<T>& p) const { return ((p.x-c.x)*(p.x-c.x)+(p.y-c.y)*(p.y-c.y)) < r*r; }
    bool isInside(const T& xp, const T& yp) const { return isInside(Vector2<T>(xp,yp)); }

    std::vector<Vector2<T>> intersections(const Ball<T>& other) const{
      std::vector<Vector2<T>> intersections;
      intersections.reserve(2);

      T d = c.getDistance(other.c);
      if( d>r+other.r || d < std::fabs(r-other.r))
        return intersections;

      T a = (d+(r*r-other.r*other.r)/d)/2,
        b = d-a,
        h=std::sqrt(r*r-a*a);
      Vector2<T> v = other.c-c, v_norm(v.y, -v.x);
      Vector2<T> p2 = c + a/d*(other.c-c);
      Vector2<T> p3_1 = p2 + h/d*v_norm,
                 p3_2 = p2 - h/d*v_norm ;

      intersections.push_back(p3_1);
      if( d>std::fabs(r-other.r) && d<r+other.r ){
        intersections.push_back(p3_2);
      }

      return intersections;
    }
};

// Insert a ball in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Ball<T>& ball){
  os << "center: " << ball.center() << " radius: " << ball.radius();
  return os;
}

#endif
