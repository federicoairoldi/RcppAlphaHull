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
  friend std::ostream& operator<<<T>(std::ostream& os, const HalfPlane<T>& hp);

  private:
    Rect<T> r; // contains information about the rect that defines the halfplane
    int side;  // if 1  => halfplane is y>mx+q or x>xr
               // if -1 => halfplane is y<mx+q or x<xr

  public:
    HalfPlane(): r(0,0), side(1) {}; // default creates an upper halfplane y>0
    HalfPlane(const Rect<T>& r): r(r), side(1) {};
    HalfPlane(const Rect<T>& r, bool maj): r(r), side((maj? 1: -1)) {};
    HalfPlane(const T& m, const T& q): r(m,q), side(1) {};
    HalfPlane(const T& m, const T& q, bool maj): r(m,q), side((maj? 1: -1)) {};
    HalfPlane(const T& xr): r(xr), side(1) {};
    HalfPlane(const T& xr, bool maj): r(xr), side((maj? 1: -1)) {};

    // Returns if the given belongs to the halfplane or not
    bool isIn(const Vector2<T>& p) const { return sign<T>(r.eval(p)) == side; };
    bool isVertical() const { return r.isVertical(); };

    T rectSlope() const { return r.slope(); };
    T rectIntercept() const { return r.intercept(); };
    int getSide() const { return side; };
};

#endif
