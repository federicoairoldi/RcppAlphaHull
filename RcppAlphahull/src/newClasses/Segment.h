#include "../MyGAL/Vector2.h"
#include "Rect.h"
#include <limits>
#include <ostream>
using namespace mygal;

#ifndef _SEGMENT_
#define _SEGMENT_

template<typename T> class Segment;

/* Returns the sign of the given value:
 * - 1 if positive
 * - -1 if negative
 * - 0 if null
 */
template<typename T>
int sign(const T& value){
  if(value > T(0))
    return 1;
  if(value < T(0))
    return -1;
  return 0;
}

// Given a point and a segment which both lie on the same rect, check whether or not the
// point falls inside the segment.
template<typename T>
bool inRange(const Segment<T>& seg, const Vector2<T>& point){
  T inf=std::min(seg.p.x, seg.q.x), sup=std::max(seg.p.x, seg.q.x);
  if( point.x < inf | sup < point.x )
    return false;

  inf=std::min(seg.p.y, seg.q.y); sup=std::max(seg.p.y, seg.q.y);
  if( point.y < inf | sup < point.y )
    return false;

  return true;
}

template<typename T>
class Segment{
  public:
    Vector2<T> p, q;

    // Constructors
    Segment(const Vector2<T>& p, const Vector2<T>& q): p(p), q(q) {}
    Segment(const T& xp, const T& yp, const T& xq, const T& yq): p(xp,yp), q(xq,yq) {}
    Segment() = delete;

    // Returns the slope of the corresponding rect for the segment: I use as convention a +inf slope for
    // rects of the form x = const
    T slope() const {
      return (isVertical()? std::numeric_limits<T>::infinity(): (q.y-p.y)/(q.x-p.x));
    }

    // Returns the intercept of the corresponding rect for the segment: I return the x coordinate of the
    // points in the case that the segment is vertical
    T intercept() const {
      return (isVertical()? -std::numeric_limits<T>::infinity(): q.y-q.x*(q.y-p.y)/(q.x-p.x));
    }

    // Returns the direction rect for the segment
    Rect<T> getRect() const{ return (!isVertical()? Rect<T>(slope(), intercept()): Rect<T>(p.x)); }

    // Returns true if the rect corresponding to the segment has form x = const
    bool isVertical() const {return p.x == q.x;}

    // Returns wheter or not two segment intersect (in R2)
    bool intersect(const Segment<T>& other) const{
      // retrieving rects
      Rect<T> r1=getRect(), r2=other.getRect();

      // check position of 2nd segment w.r.t. the 1st: if val1 and val2 have different signs then the
      // segment s2 crosses the rect r1
      T val1 = r1.eval(other.p);
      T val2 = r1.eval(other.q);
      // check position of 1st segment w.r.t. the 2nd: if val3 and val4 have different signs then the
      // segment s1 crosses the rect r2
      T val3 = r2.eval(p);
      T val4 = r2.eval(q);

      if(sign<T>(val1)!=sign<T>(val2) && sign<T>(val3)!=sign<T>(val4))
        return true;

      // colinear case
      if( val1 == 0 && val2 == 0 )
        return ( inRange<T>(Segment<T>(p,q), other.p) | inRange<T>(Segment<T>(p,q), other.q) );

      return false;
    }

    /* Returns whether or not the given point falls in the segment
     * first evaluate whether or not the point is on the rect
     * secondly check that the point coordinates fall in the same range of the segment span
     * if both condition are true then the point is in the segment
     */
    bool inside(const Vector2<T>& point){ return getRect().eval(point)==0 && inRange(*this, point); };
};

template<typename T>
bool intersect(const Segment<T>& s1, const Segment<T>& s2){
  return s1.intersect(s2);
}

// Insert a segment in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Segment<T>& seg){
  os << seg.p << "<->" << seg.q;
  return os;
}

#endif
