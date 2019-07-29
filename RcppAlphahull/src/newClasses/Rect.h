#include <limits.h>
#include <ostream>
#include <cmath>
#include "../MyGAL/Vector2.h"
using namespace mygal;

#ifndef _RECT_
#define _RECT_

template<typename T> int sign(const T& value);
template<typename T> class Segment;

template<typename T>
class Rect{
  private:
    T m, q; // slope and intercept of the rect
    bool vertical; // if true the rect has form (x = xa) and m = q = xa

  public:
    Rect()=delete;
    Rect(const T& m, const T& q): q(q), m(m), vertical(false) {}; // assuming none of m and q is infinite
    Rect(const T& xr): m(xr), q(xr), vertical(true) {}

    T slope() const { return m; };
    T intercept() const { return q; };
    bool isVertical() const { return vertical; };
    T x_r() const { return (vertical? m: std::numeric_limits<T>::quiet_NaN()); };

    /* Returns on which side of the rect the point is:
     * -  1 => y_p - (m*x_p + q) > 0 (x_p > x for vertical rects)
     * - -1 => y_p - (m*x_p + q) < 0 (x_p < x for vertical rects)
     * -  0 => y_p - (m*x_p + q) = 0 (x_p = x for vertical rects)
     */
    int eval(const Vector2<T> point) const{
      T val;
      if(vertical == false)
        val = point.y - (m*point.x+q);
      else
        val = (point.x - m);
      return sign<T>(val);
    }

    // Provided a x abscissa returns the y coordinate, if the rect is vertical and x!=xr then a quite_NaN
    // is returned, otherwise it returns the same value x.
    T eval(const T& x) const{
      if(!vertical)
        return m*x+q;
      return (x == m? x: std::numeric_limits<T>::quiet_NaN());
    }

    // Compute rect/point distance
    T getDistance(const Vector2<T>& p) const{
      return !vertical? std::fabs(p.y - eval(p.x))/std::sqrt(1+m*m): (p.x-x_r())*(p.x-x_r());
    }

    // Returns the points of the rect with distance d from the provided point, if such points don't exist
    // the returned vector is empty
    std::vector<Vector2<T>> getDistNeigh(const Vector2<T>& p, const T& d){
      T delta = d*d - std::pow(getDistance(p),2);
      if(!vertical)
        delta*=1+m*m;

      // if the provided distance d is less than the distance between the rect and the point p, we can't
      // find such pair of points, this translates in the fact that delta < 0
      if(delta < 0)
        return std::vector<Vector2<T>>();

      // int his case there's only one point
      if(delta == 0){
        T x = !vertical? (p.x-m*(q-p.y))/(1+m*m): x_r();
        T y = !vertical? m*x+q: p.y;
        return std::vector<Vector2<T>>{Vector2<T>(x,y)};
      }

      T x1 = !vertical? (p.x-m*(q-p.y)+std::sqrt(delta))/(1+m*m): x_r(),
        x2 = !vertical? (p.x-m*(q-p.y)-std::sqrt(delta))/(1+m*m): x_r();
      T y1 = !vertical? m*x1+q: p.y+std::sqrt(delta),
        y2 = !vertical? m*x2+q: p.y-std::sqrt(delta);

      return std::vector<Vector2<T>>{Vector2<T>(x1,y1), Vector2<T>(x2,y2)};
    }
};

// Insert a rect in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& r){
  if(!r.isVertical())
    os << "y = " << r.slope() << "*x " << (r.intercept()>0? "+ ": " ") << r.intercept();
  else
    os << "x = " << r.x_r();
  return os;
}

#endif
