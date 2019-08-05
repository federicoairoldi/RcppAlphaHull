#ifndef _BALL_
#define _BALL_

#include <ostream>
#include "../MyGAL/Vector2.h"
using namespace mygal;

template<typename T> class Ball;
template<typename T> class Arc;

// Insert a ball in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Ball<T>& ball){
  os << "center: " << ball.c << " radius: " << ball.r;
  return os;
}

template<typename T>
class Ball{
  friend std::ostream& operator<<<T>(std::ostream& os, const Ball<T>& ball);
  typedef Vector2<T> vector;
  typedef Arc<T> arc;
  
  private:
    vector c; // center of the ball
    T r; // radius of the ball

  public:
    // CONSTRUCTORS
    Ball(): c(0,0), r(0) {}; // default creates a degenerate ball centered in O with 0 radius
    Ball(const vector& c, const T& r): c(c), r(r) {};
    Ball(const T& xc, const T& yc, const T& r): c(xc,yc), r(r) {};

    // GETTERS
    vector center() const { return c; };
    T radius() const { return r; };
    
    // OTHER METHODS
    bool isIn(const vector& p) const { return isIn(p.x,p.y); };
    bool isIn(const T& xp, const T& yp) const { return ((xp-c.x)*(xp-c.x)+(yp-c.y)*(yp-c.y)) < r*r; }
    bool isOnBoundary(const vector& p) const { return isOnBoundary(p.x,p.y); };
    // maybe consider to use tollerance here too
    bool isOnBoundary(const T& xp, const T& yp) const { return ((xp-c.x)*(xp-c.x)+(yp-c.y)*(yp-c.y)) == r*r; };
    bool operator==(const Ball& b2){
      if(c.x!=b2.c.x || c.y!=b2.c.y || r!=b2.r)
        return false;
      return true;
    }
    
    // compute intersection points between two balls with different centers
    std::vector<vector> intersections(const Ball<T>& other) const{
      // maybe throw an error if a ball with the same center ios provided
      
      std::vector<vector> intersections;
      intersections.reserve(2);

      T d = c.getDistance(other.c);
      if( d>r+other.r || d < std::fabs(r-other.r))
        return intersections;

      T a = (d+(r*r-other.r*other.r)/d)/2,
        b = d-a,
        h=std::sqrt(r*r-a*a);
      // retrieving the vector v and its orthogonal (both have norm equal to d)
      vector v = other.c-c,
             v_norm = v.getOrthogonal();
      // retrieving the dummy point on the segment between the two balls' centers
      vector p2 = c + a/d*(other.c-c);
      // computing the two intersections
      vector p3_1 = p2 - h/d*v_norm,
             p3_2 = p2 + h/d*v_norm ;

      intersections.push_back(p3_1);
      if( d>std::fabs(r-other.r) && d<r+other.r ){
        intersections.push_back(p3_2);
      }

      return intersections;
    }
    
    // compute intersection arc between two balls with different centers (the arc is on the caller circumference)
    arc intersection_arc(const Ball<T>& other) const{
      std::vector<vector> points = intersections(other);
      
      if(points.size()<2)
        return arc(*this,1,0,0); // returning a null arc
      
      // if there are two intersection points then i can define an arc
      return arc(*this,points[1],0);
    }
};

template<typename T>
std::vector<Vector2<T>> intersections(const Ball<T>& b1, const Ball<T>& b2){ return b1.intersections(b2); }

#endif
