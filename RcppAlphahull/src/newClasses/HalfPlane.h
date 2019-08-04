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
  
  friend std::ostream& operator<<<T>(std::ostream& os, const HalfPlane<T>& hp);

  private:
    rect r; // contains information about the rect that defines the halfplane
    int side;  // if 1  => halfplane is y>mx+q or x>xr
               // if -1 => halfplane is y<mx+q or x<xr

  public:
    // CONSTRUCTORS
    HalfPlane(): r(0,0), side(1) {}; // default creates an upper halfplane y>0
    HalfPlane(const rect& r, bool maj = true): r(r), side((maj? 1: -1)) {};
    HalfPlane(const T& m, const T& q, bool maj = true): r(m,q), side((maj? 1: -1)) {};
    HalfPlane(const T& xr, bool maj = true): r(xr), side((maj? 1: -1)) {};

    // OTHER METHODS
    // Returns if the given belongs to the halfplane or not
    bool isIn(const vector& p) const { return sign<T>(r.eval(p)) == side; };
    bool isVertical() const { return r.isVertical(); };
    bool isHorizontal() const { return r.isHorizontal(); };

    T rectSlope() const { return r.slope(); };
    T rectIntercept() const { return r.intercept(); };
    int getSide() const { return side; };
};

#endif
