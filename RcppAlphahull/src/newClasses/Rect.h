#ifndef _RECT_
#define _RECT_

#include <limits.h>
#include <ostream>
#include <cmath>
#include "../MyGAL/Vector2.h"
#include "geomUtil.h"
using namespace mygal;

template<typename T> class Segment;
template<typename T> class Rect;

// Insert a rect in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& r){
  os << r.a << "*y +" << r.b << "*x + " << r.c;
  return os;
}

template<typename T>
class Rect{
  typedef Vector2<T> vector;
  
  // FRIENDS
  friend std::ostream& operator<<<T>(std::ostream& os, const Rect<T>& r);

  // ATTRIBUTES
  private:
    T a,b,c; // a*y + b*x + c = 0 -> m=-b/a and q = -c/a for non vertical rects

    /* normalizes the rect:
     * - a<0 => switches sings of a, b and c
     * - a=0 and b<0 => switches sings of b and c
     */
    void normalize(){
      if(a<0 || (a==0 && b<0)){
        a=-a; b=-b; c=-c;
      }
    };

  public:
    // CONSTRUCTORS
    Rect()=delete;
    Rect(const T& m, const T& q): a(1), b(-m), c(-q) {};
    Rect(const T& xr): a(0), b(1), c(-xr) {};
    Rect(const T& a, const T& b, const T& c): a(a), b(b), c(c) {
      if(std::fabs(a)+std::fabs(b) == 0)
        std::cerr << "Error! both a nd b are null" << std::endl;
      normalize(); // normalizes the rect
    }; // raises an error if both a and b are 0
    Rect(const vector& p1, const vector& p2): Rect(p2.x-p1.x, p1.y-p2.y, p1.x*p2.y-p2.x*p1.y)
    { if( p1==p2 ) std::cerr << "Error! p1 and p2 are the same point" << std::endl; };

    // GETTERS
    T slope() const { return a!=0? -b/a: std::numeric_limits<T>::infinity(); };
    T intercept() const { return a!=0? -c/a: -std::numeric_limits<T>::infinity(); };
    bool isVertical() const { return a==0; };
    bool isHorizontal() const { return b==0; };
    // for vertical lines returns the abscissa of its equation, if the line is not vertical it returns a NaN
    T x_r() const { return (isVertical()? -c/b: std::numeric_limits<T>::quiet_NaN()); };

    // OTHER METHODS
    /* Returns on which side of the rect the point is:
     * -  1 => a*y_p + b*x_p + c > 0 ( y_p - (m*x_p + q) > or x_p > x for vertical rects)
     * -  0 => a*y_p + b*x_p + c = 0 ( y_p - (m*x_p + q) = or x_p = x for vertical rects)
     * - -1 => a*y_p + b*x_p + c < 0 ( y_p - (m*x_p + q) < or x_p < x for vertical rects)
     */
    int eval(const vector& point) const{ return sign<T>(a*point.y + b*point.x + c); }

    // Provided a x abscissa returns the y coordinate, if the rect is vertical and x!=xr then a quite_NaN
    // is returned, otherwise it returns the same value x.
    T eval(const T& x) const{
      if(!isVertical())
        return -(b*x+c)/a;
      return std::numeric_limits<T>::quiet_NaN();
    }

    // Compute rect/point distance
    T getDistance(const vector& p) const{ return std::fabs(a*p.y + b*p.x + c)/std::sqrt(a*a+b*b); }

    // Returns the points of the rect with distance d from the provided point, if such points don't exist
    // the returned vector is empty
    std::vector<vector> getDistNeigh(const vector& p, const T& d){
      T delta = d*d - std::pow(getDistance(p),2), m = slope(), q = intercept();
      if(!isVertical())
        delta*=1+m*m;

      // if the provided distance d is less than the distance between the rect and the point p, we can't
      // find such pair of points, this translates in the fact that delta < 0
      if(delta < 0)
        return std::vector<vector>();

      // int his case there's only one point
      if(delta == 0){
        T x = !isVertical()? (p.x-m*(q-p.y))/(1+m*m): x_r();
        T y = !isVertical()? m*x+q: p.y;
        return std::vector<vector>{vector(x,y)};
      }

      T x1 = !isVertical()? (p.x-m*(q-p.y)-std::sqrt(delta))/(1+m*m): x_r(),
        x2 = !isVertical()? (p.x-m*(q-p.y)+std::sqrt(delta))/(1+m*m): x_r();
      T y1 = !isVertical()? m*x1+q: p.y-std::sqrt(delta),
        y2 = !isVertical()? m*x2+q: p.y+std::sqrt(delta);

      return std::vector<vector>{vector(x1,y1), vector(x2,y2)};
    };
};


#endif
