#ifndef _LINE_
#define _LINE_

#include <limits.h>
#include <ostream>
#include <cmath>
#include "../MyGAL/Vector2.h"
#include "geomUtil.h"
using namespace mygal;

template<typename T> class Segment;
template<typename T> class Line;

// Inserts a line in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Line<T>& r){
  os << r.a << "*y +" << r.b << "*x + " << r.c;
  return os;
}

template<typename T>
class Line{
  typedef Vector2<T> vector;
  
  // FRIENDS
  friend std::ostream& operator<<<T>(std::ostream& os, const Line<T>& r);

  // ATTRIBUTES
  private:
    T a,b,c; // a*y + b*x + c = 0 -> m=-b/a and q = -c/a for non vertical lines

    /* normalizes the line:
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
    Line() = delete;
    Line(const T& m, const T& q): a(1), b(-m), c(-q) {};
    Line(const T& xr): a(0), b(1), c(-xr) {};
    Line(const T& a, const T& b, const T& c): a(a), b(b), c(c) {
      if( std::fabs(a) + std::fabs(b) == 0 ) // can't define a line if both a and b are null
        std::cerr << "Error! both a nd b are null" << std::endl;
      normalize(); // normalizes the line
    }; // raises an error if both a and b are 0
    Line(const vector& p1, const vector& p2): Line(p2.x-p1.x, p1.y-p2.y, p1.x*p2.y-p2.x*p1.y)
    { if( p1 == p2 ) std::cerr << "Error! p1 and p2 are the same point" << std::endl; };

    // GETTERS
    T getA() const { return a; }
    T getb() const { return a; }
    T getc() const { return a; }
    T slope() const { return a!=0? -b/a: std::numeric_limits<T>::infinity(); }; // Returns the slope of the line
    T intercept() const { return a!=0? -c/a: -std::numeric_limits<T>::infinity(); }; // Returns the intercept of the line
    bool isVertical() const { return a==0; }; // Returns true if the line is vertical (x>x_r or x<x_r)
    bool isHorizontal() const { return b==0; };
    // for vertical lines returns the abscissa of its equation, if the line is not vertical it returns a NaN
    T x_r() const { return (isVertical()? -c/b: std::numeric_limits<T>::quiet_NaN()); };

    // OTHER METHODS
    /* Returns on which side of the line the point is:
     * -  1 => a*y_p + b*x_p + c > 0 ( y_p - (m*x_p + q) > or x_p > x for vertical lines)
     * -  0 => a*y_p + b*x_p + c = 0 ( y_p - (m*x_p + q) = or x_p = x for vertical lines)
     * - -1 => a*y_p + b*x_p + c < 0 ( y_p - (m*x_p + q) < or x_p < x for vertical lines)
     */
    int eval(const vector& p) const{ return sign<T>(a*p.y + b*p.x + c); }

    // Provided a x abscissa returns the y coordinate, if the line is vertical and x!=xr then a quite_NaN
    // is returned, otherwise it returns the same value x.
    T eval(const T& x) const{
      if(!isVertical())
        return -(b*x+c)/a;
      return std::numeric_limits<T>::quiet_NaN();
    }

    // Compute line/point distance
    T getDistance(const vector& p) const{ return std::fabs(a*p.y + b*p.x + c)/std::sqrt(a*a+b*b); }

    // Returns the points of the line with distance d from the provided point, if such points don't exist
    // the returned vector is empty
    std::vector<vector> getDistNeigh(const vector& p, const T& d){
      T delta = d*d - std::pow(getDistance(p),2), m = slope(), q = intercept();
      if(!isVertical())
        delta*=1+m*m;

      // if the provided distance d is less than the distance between the line and the point p, we can't
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
