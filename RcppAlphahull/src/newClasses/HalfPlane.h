#ifndef _HALFPLANE_
#define _HALFPLANE_

#include <ostream>
#include "Rect.h"
#include "../MyGAL/Vector2.h"

// class to define open halfplanes in R2

template<typename T> class HalfPlane;

// Insert an halfplane in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const HalfPlane<T>& hp){
  if(!hp.r.isVertical())
    os << "y " << (hp.getSide()>0? "> ": "< ") << hp.rectSlope() << "*x " << (hp.rectIntercept()>0? "+ ": " ") << hp.rectIntercept();
  else
    os << "x " << (hp.getSide()>0? "> ": "< ") << hp.r.x_r();
  return os;
}

template<typename T>
class HalfPlane{
  typedef Rect<T> rect;
  typedef Vector2<T> vector;
  
  // FRIENDS
  friend std::ostream& operator<<<T>(std::ostream& os, const HalfPlane<T>& hp);

  private:
    // ATTRIBUTES
    rect r; // contains information about the rect that defines the halfplane
    int side;  // if 1  => halfplane is y>mx+q or x>xr
               // if -1 => halfplane is y<mx+q or x<xr

  public:
    // CONSTRUCTORS
    HalfPlane(): r(0,0), side(1) {}; // default creates an upper halfplane y>0
    HalfPlane(const rect& r, bool maj = true): r(r), side((maj? 1: -1)) {};
    HalfPlane(const T& m, const T& q, bool maj = true): r(m,q), side((maj? 1: -1)) {};
    HalfPlane(const T& xr, bool maj = true): r(xr), side((maj? 1: -1)) {};
    
    // GETTERS
    T rectSlope() const { return r.slope(); };
    T rectIntercept() const { return r.intercept(); };
    rect getRect() const{ return r; };
    int getSide() const { return side; };
    
    // OTHER METHODS
    // Returns if the given point belongs to the halfplane or not
    bool isIn(const vector& p) const { return sign<T>(r.eval(p)) == side; };
    bool isIn(const T& xp, const T& yp) const { return isIn(vector(xp,yp)); };
    // Returns if the given point is on the boundaryof the halfplane or not
    bool isOnBoundary(const vector& p) const { return sign<T>(r.eval(p)) == 0; };
    bool isOnBoundary(const T& xp, const T& yp) const { return isOnBoundary(vector(xp,yp)); };
    // Returns if the halfplane is a vertical one
    bool isVertical() const { return r.isVertical(); };
    // Returns if the halfplane is an horizontral one
    bool isHorizontal() const { return r.isHorizontal(); };
};

#endif
