#include "../MyGAL/Vector2.h"
#include "Rect.h"
#include <limits>
#include <ostream>
using namespace mygal;

#ifndef _SEGMENT_
#define _SEGMENT_

template<typename T> class Segment;

// Given a point and a segment which both lie on the same rect, check whether or not the
// point falls inside the segment.
template<typename T>
bool inRange(const Segment<T>& seg, const Vector2<T>& point){
  T inf=std::min(seg.p.x, seg.q.x), sup=std::max(seg.p.x, seg.q.x);
  if( point.x <= inf || sup <= point.x )
    return false;

  inf=std::min(seg.p.y, seg.q.y); sup=std::max(seg.p.y, seg.q.y);
  if( point.y <= inf || sup <= point.y )
    return false;

  return true;
}

// Insert a segment in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Segment<T>& seg){
  os << seg.p << "<->" << seg.q;
  return os;
}

// class to define open segments in R2
template<typename T>
class Segment{
  typedef Vector2<T> vector;
  typedef Rect<T> rect;
  
  // FRIENDS
  friend bool inRange<T>(const Segment<T>& seg, const vector& point);
  friend std::ostream& operator<<<T>(std::ostream& os, const Segment<T>& seg);

  private:
    // ATTRIBUTES
    vector p, q;

  public:
    // CONSTRUCTORS
    Segment(const vector& p, const vector& q): p(p), q(q) {}
    Segment(const T& xp, const T& yp, const T& xq, const T& yq): p(xp,yp), q(xq,yq) {}
    Segment() = delete;

    // GETTERS
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
    rect getRect() const{ return rect(p,q); }

    // Returns true if the rect corresponding to the segment has form x = const
    bool isVertical() const {return p.x == q.x;}
    
      if(val1!=val2 && val3!=val4)
        return true;

      // colinear case
      if( val1 == 0 && val2 == 0 )
        return ( inRange<T>(Segment<T>(p,q), other.p) || inRange<T>(Segment<T>(p,q), other.q) );

      return false;
    }

    /* Returns whether or not the given point falls in the segment
     * first evaluate whether or not the point is on the rect
     * secondly check that the point coordinates fall in the same range of the segment span
     * if both condition are true then the point is in the segment
     */
    bool inside(const Vector2<T>& point) const { return getRect().eval(point)==0 && inRange(*this, point); };
};

template<typename T>
bool intersect(const Segment<T>& s1, const Segment<T>& s2){
  return s1.intersect(s2);
}

template<typename T>
bool inside(const Segment<T>& seg, const Vector2<T>& point) { return seg.inside(point); }

#endif
