#ifndef _ARC_
#define _ARC_

#include <ostream>
#include "Ball.h" 
#include "../MyGAL/Vector2.h"
using namespace mygal;

template<typename T> class Arc;

// returns a vector rotated (counter-clockwise) by the angle theta
template<typename T>
Vector2<T> rotate(const Vector2<T>& v, const T& theta){
  T x2 = v.x*std::cos(theta)-v.y*std::sin(theta); 
  T y2 = v.x*std::sin(theta)+v.y*std::cos(theta); 
  return Vector2<T>(x2,y2);
}

// returns an arc rotated (counter-clockwise) by the angle theta
template<typename T>
Arc<T> rotate(const Arc<T>& a, const T& theta){
  return Arc<T>(a.b, rotate(a.v,theta), a.alpha);
}

// Insert an arc in a stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Arc<T>& a){
  os << "center: " << a.b.center() << " radius: " << a.b.radius() << " v: " << a.v << " alpha: " 
     << a.alpha;
  return os;
}

template<typename T>
class Arc{
  typedef Ball<T> ball;
  typedef Vector2<T> vector;
  
  friend Arc<T> rotate<T>(const Arc<T>& a, const T& theta);
  friend std::ostream& operator<<<T>(std::ostream& os, const Arc<T>& a);
  
  private:
    ball b; // ball
    vector v; // vector pointing to the start of the arc
    T alpha; // 
  
  public:
    // CONSTRUCTORS
    Arc() = delete; // deleting default constructor
    Arc(const ball& b): b(b), v(vector(1,0)), alpha(2*M_PI) {}; // defines an arc that covers the whole circle
    Arc(const ball& b, const vector& v, const T& alpha = 2*M_PI): b(b), v(1/v.getNorm()*v), alpha(alpha) 
    { if(alpha<0 || alpha>2*M_PI) std::cerr << "Error! Magnitude for angle not in [0;2*pi]"; };
    Arc(const ball& b, const T& vx, const T& vy, const T& alpha = 2*M_PI): Arc(b, vector(vx,vy), alpha) {};
    Arc(const T& cx, const T& cy,const T& r, const T& vx = 1, const T& vy = 0, const T& alpha = 2*M_PI): 
      b(cx, cy, r), v(vx, vy), alpha(alpha) {};
    
    // GETTERS
    ball getBall() const { return b; };
    vector getVector() const { return v; }; // a vector pointing the starting of the arc from center
    vector getEndVector() const { return rotate(v,alpha); }; // a vector pointing the ending of the arc from center
    T width() const { return alpha; };
    
    // OTHER METHODS
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
    
    // compute the set difference between two arcs on the same circumference
    std::vector<Arc> diff(const Arc& a2) const{// arcs not on the same circumference
      if( (b.center().x!=a2.b.center().x) || (b.center().y!=a2.b.center().y)){
        std::cerr << "Error! arc difference with arcs in differet circles" << std::endl;
        return std::vector<Arc>{*this};
      }
      
      // second arc is the whole circumference (the whole arc is removed)
      if(a2.alpha == 2*M_PI ) // I have to remove a whole circumference so the arc vanishes
        return std::vector<Arc>();
        
      // second arc is a null arc (I have to do nothing)
      if(a2.alpha==0)
        return std::vector<Arc>{*this};
      
      if(theta1()!=0){
        std::vector<Arc> res = rotate(*this, -theta1()).diff(rotate(a2, -theta1()));
        for(size_t i=0; i<res.size(); i++)
          res[i] = rotate(res[i], theta1());
        return res;
      }
      
      // if we arrive here then we have that theta1 of first arc is 0.
      std::cout << "a1 theta1: " << theta1() << std::endl;
      std::cout << "a1 theta2: " << theta2() << std::endl;
      std::cout << "a2 theta1: " << a2.theta1() << std::endl;
      std::cout << "a2 theta2: " << a2.theta2() << std::endl;
      
      // case 1: 0 < alpha < a2.theta1 < a2.theta2 (0 < alpha is always true by construction)
      if( alpha < a2.theta1() && a2.theta1() < a2.theta2() ){
        std::cout << "case 1" << std::endl;
        return std::vector<Arc>{*this};
      }
      
      // case 2: 0 < a2.theta1 < alpha < a2.theta2
      if( 0 < a2.theta1() && a2.theta1() < alpha && alpha < a2.theta2() ){
        std::cout << "case 2" << std::endl;
        return std::vector<Arc>{Arc(b,v,a2.theta1())};
      }
      
      // case 3: 0 < a2.theta1 < a2.theta2 < alpha
      if( 0 < a2.theta1() && a2.theta1() < a2.theta2() && a2.theta2() < alpha){
        std::cout << "case 3" << std::endl;
        // I have to split the arc in two parts
        Arc arcNew1(b, v, a2.theta1()), arcNew2(b, a2.getEndVector(), alpha-a2.theta2());
        return std::vector<Arc>{arcNew1, arcNew2};
      }
      
      // case 4: 0 = a2.theta1 < alpha < a2.theta2
      if(a2.theta1() == 0 && alpha <= a2.theta2()){
        std::cout << "case 4" << std::endl;
        return std::vector<Arc>();
      }
      
      // case 5: 0 = a2.theta1 < alpha < a2.theta2
      if(a2.theta1() == 0 && a2.theta2() < alpha){
        std::cout << "case 5" << std::endl;
        return std::vector<Arc>{Arc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // case 6: 0 < a2.theta2 < alpha < a2.theta1
      if( 0 < a2.theta2() && a2.theta2() < alpha && alpha < a2.theta1() ){
        std::cout << "case 6" << std::endl;
        return std::vector<Arc>{Arc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // case 8: 0 < alpha < a2.theta2 < a2.theta1
      if(alpha <= a2.theta2() && a2.theta2() < a2.theta1()){
        std::cout << "case 8" << std::endl;
        return std::vector<Arc>();;
      }
      
      // case 9: 0 < a2.theta2 < a2.theta1 < alpha
      if(a2.theta2() < a2.theta1() && a2.theta1() <= alpha){
        std::cout << "case 9" << std::endl;
        return std::vector<Arc>{Arc(b, a2.getEndVector(), a2.theta1()-a2.theta2())};
      }
      
      // NB: to be removed if no errors occur in future
      // case 7: 0 = a2.theta1 < alpha < a2.theta2 (maybe this shouldn't happen)
      if(a2.theta2() == 0 && a2.theta2() < alpha){
        std::cout << "case 7" << std::endl;
        return std::vector<Arc>{Arc(b, a2.getEndVector(), alpha-a2.theta2())};
      }
      
      // just in case i have done something wrong...
      std::cerr << "No case match!!" << std::endl;
      return std::vector<Arc>();
    };
};

#endif
