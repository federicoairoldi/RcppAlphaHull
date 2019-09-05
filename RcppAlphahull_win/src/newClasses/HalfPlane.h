#ifndef _HALFPLANE_
#define _HALFPLANE_

#include <cmath>
#include <ostream>
#include "AreaObj.h"
#include "Line.h"
#include "../MyGAL/Vector2.h"

// class to define open halfplanes in R2

template<typename T> class HalfPlane;

// Inserts an halfplane in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const HalfPlane<T>& hp){
  if(!hp.r.isVertical())
    os << "y " << (hp.getSide()>0? "> ": "< ") << hp.lineSlope() << "*x " << (hp.lineIntercept()>0? "+ ": " ") << hp.lineIntercept();
  else
    os << "x " << (hp.getSide()>0? "> ": "< ") << hp.r.x_r();
  return os;
}

template<typename T>
class HalfPlane: public AreaObj<T>{
  typedef Line<T> line;
  typedef Vector2<T> vector2;
  
  // FRIENDS
  friend std::ostream& operator<<<T>(std::ostream& os, const HalfPlane<T>& hp);

  private:
    // ATTRIBUTES
    line r; // contains information about the line that defines the halfplane
    int side;  // if 1  => halfplane is y>mx+q or x>xr
               // if -1 => halfplane is y<mx+q or x<xr

  public:
    // CONSTRUCTORS
    HalfPlane() = delete; // default creates an upper halfplane y>0
    HalfPlane(const line& r, bool maj = true): r(r), side((maj? 1: -1)) {};
    HalfPlane(const T& m, const T& q, bool maj = true): r(m,q), side((maj? 1: -1)) {};
    HalfPlane(const T& xr, bool maj = true): r(xr), side((maj? 1: -1)) {};
    
    // GETTERS
    T lineSlope() const { return r.slope(); };
    T lineIntercept() const { return r.intercept(); };
    line getLine() const{ return r; };
    int getSide() const { return side; };
    // Returns the area of the object
    T area() const override { return std::numeric_limits<T>::infinity(); };
    
    // OTHER METHODS
    // Returns if the given point belongs to the halfplane or not
    bool isIn(const vector2& p) const override { return sign<T>(r.eval(p)) == side; };
    bool isIn(const T& xp, const T& yp) const override { return isIn(vector2(xp,yp)); };
    // Returns if the given point is on the boundary of the halfplane or not
    bool isOnBound(const vector2& p) const override { return sign<T>(r.eval(p)) == 0; };
    bool isOnBound(const T& xp, const T& yp) const override { return isOnBound(vector2(xp,yp)); };
    // Returns if the halfplane is a vertical one
    bool isVertical() const { return r.isVertical(); };
    // Returns if the halfplane is an horizontral one
    bool isHorizontal() const { return r.isHorizontal(); };
};

#endif
