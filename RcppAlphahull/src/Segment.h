#include "MyGAL/Vector2.h"
#include <limits>
using namespace mygal;

template<typename T>
int sign(const T& value){
  if(value > T(0))
    return 1;
  if(value < T(0))
    return -1;
  return 0;
}

// Check if the point lies on the segment
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

    T slope() const {
      return (isVertical()? std::numeric_limits<T>::infinity(): (q.y-p.y)/(q.x-p.x));
    }

    T intercept() const {
      return (isVertical()? -std::numeric_limits<T>::infinity(): q.y-q.x*(q.y-p.y)/(q.x-p.x));
    }

    bool isVertical() const {return p.x == q.x;}

    // Returns wheter or not two segment intersect (in R2)
    bool intersect(const Segment<T>& other) const{
      T val1, val2, val3, val4;
      T m1 = slope(), c1 = intercept(), m2 = other.slope(), c2 = other.intercept();

      // rect of 1st segment is y = m1*x + c1, of 2nd segment is y = m2*x + c2
      if(m1==std::numeric_limits<T>::infinity() & m2==std::numeric_limits<T>::infinity()){
        if(p.x != other.p.x)
          return false;
        else
          return (inRange<T>( Segment<T>(p, q),  other.p) | inRange<T>( Segment<T>(p, q),  other.q));
      }

      // check position of 2nd segment w.r.t. the 1st
      val1 = (m1!=std::numeric_limits<T>::infinity()? other.p.y - (m1*other.p.x + c1): other.p.x<p.x );
      val2 = (m1!=std::numeric_limits<T>::infinity()? other.q.y - (m1*other.q.x + c1): other.q.x<p.x );
      // check position of 1st segment w.r.t. the 2nd
      val3 = (m2!=std::numeric_limits<T>::infinity()? p.y - (m2*p.x + c2): p.x<other.p.x );
      val4 = (m2!=std::numeric_limits<T>::infinity()? q.y - (m2*q.x + c2): q.x<other.p.x );

      if(sign<T>(val1)!=sign<T>(val2) & sign<T>(val3)!=sign<T>(val4))
        return true;

      // colinear case
      if( val1 == 0 & val2 == 0 )
        return (inRange<T>( Segment<T>(p, q),  other.p) | inRange<T>( Segment<T>(p, q),  other.q));

      return false;
    }
};

template<typename T>
bool intersect(const Segment<T>& s1, const Segment<T>& s2){
  return s1.intersect(s2);
}

/* Returns the orientation of an ordered set of three points (p1, p2, p3) in R2:
 * 0 = colinear
 * 1 = clockwise
 * 2 = counter-clockwise
 */
template<typename T>
int orientation(Vector2<T>& p1, Vector2<T>& p2, Vector2<T>& p3){
  int value = (p2.y-p1.y)*(p3.x-p2.x) - (p3.y-p2.y)*(p2.x-p1.x);
  if(value == 0)
    return 0;
  return (value > 0)? 1: 2;
}
