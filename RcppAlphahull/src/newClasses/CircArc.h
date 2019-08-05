#ifndef _ARC_
#define _ARC_

#include <ostream>
#include "Ball.h" 
#include "../MyGAL/Vector2.h"
#include "geomUtil.h"
using namespace mygal;

template<typename T> class CircArc;
template<typename T> class Ball;

// Insert an arc in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const CircArc<T>& a){
  os << "center: " << a.b.center() << " radius: " << a.b.radius() << " v: " << a.v << " alpha: " 
     << a.alpha;
  return os;
}

template<typename T>
class CircArc{
  typedef Ball<T> ball;
  typedef Vector2<T> vector;
  
  friend std::ostream& operator<<<T>(std::ostream& os, const CircArc<T>& a);
  
  private:
    ball b; // ball
    vector v; // vector pointing to the start of the arc
    T alpha; // 
  
  public:
    // CONSTRUCTORS
    CircArc() = delete; // deleting default constructor
    CircArc(const ball& b): b(b), v(vector(1,0)), alpha(2*M_PI) {}; // defines an arc that covers the whole circle
    CircArc(const ball& b, const vector& v, const T& alpha = 2*M_PI): b(b), v(1/v.getNorm()*v), alpha(alpha) 
    { if(alpha<0 || alpha>2*M_PI) std::cerr << "Error! Magnitude for angle not in [0;2*pi]"; };
    CircArc(const ball& b, const T& vx, const T& vy, const T& alpha = 2*M_PI): CircArc(b, vector(vx,vy), alpha) {};
    CircArc(const T& cx, const T& cy,const T& r, const T& vx = 1, const T& vy = 0, const T& alpha = 2*M_PI): 
      b(cx, cy, r), v(vx, vy), alpha(alpha) {};
    
    // GETTERS
    ball getBall() const { return b; };
    vector getVector() const { return v; }; // a vector pointing the starting of the arc from center
    vector getEndVector() const { return rotate<T>(v,alpha); }; // a vector pointing the ending of the arc from center
    T width() const { return alpha; };
    
    // OTHER METHODS
    // computes the length of the arc
    T length() const { return alpha*b.radius(); };
    
    // return the angle of v, starting point of the arc (value between 0 and 2*PI)
    T theta1() const { 
      T theta1 = std::atan2(v.y, v.x);
      theta1+= theta1<0? 2*M_PI: 0;
      return theta1;
    };
    
    // return end point of the arc (value between 0 and 2*PI)
    T theta2() const { 
      T theta2 = theta1()+alpha;
      theta2-= theta2>2*M_PI? 2*M_PI: 0;
      return theta2;
    };
    
    // returns whether or not the arc is degenerate (is just one point)
    bool isNULL() const { return alpha==0; }; // maybe do it with tollerance
    
    // OPERATIONS ON ARCS
    // returns an arc rotated (counter-clockwise) by the angle theta
    CircArc<T> rotate_arc(const T& theta) const { return CircArc<T>(b, rotate<T>(v,theta), alpha); }
    
    // compute the set difference between two arcs on the same circumference
    std::vector<CircArc> diff(const CircArc& a2) const{// arcs not on the same circumference
      if( b.center()!=a2.b.center() ){
        std::cerr << "Error! arc difference with arcs in differet circles" << std::endl;
        return std::vector<CircArc>{*this};
      }
      
      // second arc is the whole circumference (the whole arc is removed)
      if(a2.alpha == 2*M_PI ) // I have to remove a whole circumference so the arc vanishes
        return std::vector<CircArc>();
        
      // second arc is a null arc (I have to do nothing)
      if(a2.alpha==0)
        return std::vector<CircArc>{*this};
      
      if(theta1()!=0){
        std::vector<CircArc> res = rotate_arc(-theta1()).diff(a2.rotate_arc(-theta1()));
        for(size_t i=0; i<res.size(); i++)
          res[i] = res[i].rotate_arc(theta1());
        return res;
      }
      
      // if we arrive here then we have that theta1 of first arc is 0.
      
      // case 1: 0 < alpha < a2.theta1 < a2.theta2 (0 < alpha is always true by construction)
      if( alpha < a2.theta1() && a2.theta1() < a2.theta2() ){
        // std::cout << "case 1" << std::endl;
        return std::vector<CircArc>{*this};
      }
      
      // case 2: 0 < a2.theta1 < alpha < a2.theta2
      if( 0 < a2.theta1() && a2.theta1() < alpha && alpha < a2.theta2() ){
        // std::cout << "case 2" << std::endl;
        return std::vector<CircArc>{CircArc(b,v,a2.theta1())};
      }
      
      // case 3: 0 < a2.theta1 < a2.theta2 < alpha
      if( 0 < a2.theta1() && a2.theta1() < a2.theta2() && a2.theta2() < alpha){
        // std::cout << "case 3" << std::endl;
        // I have to split the arc in two parts
        if( alpha == 2*M_PI ) // sub-special case: instead of two arcs I just form one
          return std::vector<CircArc>{CircArc(b, a2.getEndVector(), alpha-a2.theta2()+a2.theta1())};
        
        CircArc arcNew1(b, v, a2.theta1()), arcNew2(b, a2.getEndVector(), alpha-a2.theta2());
        return std::vector<CircArc>{arcNew1, arcNew2};
      }
      
      // case 4: 0 = a2.theta1 < alpha < a2.theta2
      if(a2.theta1() == 0 && alpha <= a2.theta2()){
        // std::cout << "case 4" << std::endl;
        return std::vector<CircArc>();
      }
      
      // case 5: 0 = a2.theta1 < alpha < a2.theta2
      if(a2.theta1() == 0 && a2.theta2() < alpha){
        // std::cout << "case 5" << std::endl;
        return std::vector<CircArc>{CircArc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // case 6: 0 < a2.theta2 < alpha < a2.theta1
      if( 0 < a2.theta2() && a2.theta2() < alpha && alpha < a2.theta1() ){
        // std::cout << "case 6" << std::endl;
        return std::vector<CircArc>{CircArc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // case 8: 0 < alpha < a2.theta2 < a2.theta1
      if(alpha <= a2.theta2() && a2.theta2() < a2.theta1()){
        // std::cout << "case 8" << std::endl;
        return std::vector<CircArc>();;
      }
      
      // case 9: 0 < a2.theta2 < a2.theta1 < alpha
      if(a2.theta2() < a2.theta1() && a2.theta1() <= alpha){
        // std::cout << "case 9" << std::endl;
        return std::vector<CircArc>{CircArc(b, a2.getEndVector(), a2.theta1()-a2.theta2())};
      }
      
      // NB: to be removed if no errors occur in future
      // case 7: 0 = a2.theta1 < alpha < a2.theta2 (maybe this shouldn't happen)
      if(a2.theta2() == 0 && a2.theta2() < alpha){
        // std::cout << "case 7" << std::endl;
        return std::vector<CircArc>{CircArc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // just in case i have done something wrong...
      std::cerr << "No case match!!" << std::endl;
      return std::vector<CircArc>();
    };
    
    // remove from the caller the part of arc that falls inside the given ball
    std::vector<CircArc> removeBall(const ball& b2) const {
      if( b2.containsBall(b) ) // I have to remove the whole arc, nothing remains
        return std::vector<CircArc>{};
      if( b.containsBall(b2) ) // I have to remove nothing, the arc remains intact
        return std::vector<CircArc>{*this};
      CircArc a2 = b.intersection_arc(b2);
      if( a2.alpha>0 )
        return diff(a2);
      return  std::vector<CircArc>{*this};
    }
};

template<typename T>
std::vector<CircArc<T>> diff(const CircArc<T>& a1, const CircArc<T>& a2) { return a1.diff(a2); }

// returns an arc rotated (counter-clockwise) by the angle theta
template<typename T>
CircArc<T> rotate_arc(const CircArc<T>& a, const T& theta){ return a.rotate(theta); }

// given a vector of arcs remove from each one the parts in common with the provided ball returning
// a vector containing the new arc parts
template<typename T>
std::vector<CircArc<T>> collective_removeBall(const std::vector<CircArc<T>>& arcs, const Ball<T>& b){
  std::vector<CircArc<T>> res;
  
  for(size_t i=0; i<arcs.size(); i++){
    std::vector<CircArc<T>> tmp = arcs[i].removeBall(b); // subtracting the ball from the current element
    res.insert(res.end(), tmp.begin(), tmp.end()); // adding the subtracted arcs to the result
  }
  
  return res;
}

#endif
