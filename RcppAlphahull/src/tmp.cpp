#include <Rcpp.h>
#include <cmath>
#include "newClasses/Rect.h"
#include "newClasses/Segment.h"
#include "newClasses/Ball.h"
#include "newClasses/HalfPlane.h"
#include "newClasses/CircArc.h"
using namespace Rcpp;

// [[Rcpp::export]]
void testRcpp() {
  Rect<long double> r1(2,7), rvert(2.457489485);
  Vector2<long double> p(1,9), q(1/3, 2/3+7), t(2.457489485, 393);
  Vector2<long double> u(1/11, r1.eval(1/11));

  Rcpp::Rcout << r1.isVertical() << std::endl;
  Rcpp::Rcout << rvert.isVertical() << std::endl;

  Rcpp::Rcout << r1.eval(p) << std::endl;
  Rcpp::Rcout << r1.eval(q) << std::endl;
  Rcpp::Rcout << r1.eval(t) << std::endl;
  Rcpp::Rcout << r1.eval(u) << std::endl;

  Rcpp::Rcout << rvert.eval(p) << std::endl;
  Rcpp::Rcout << rvert.eval(q) << std::endl;
  Rcpp::Rcout << rvert.eval(t) << std::endl;

  HalfPlane<long double> h1(r1), h2(r1, false), h3(rvert);

  std::cout << r1 << std::endl;
  std::cout << rvert << std::endl;
  std::cout << h1 << std::endl;
  std::cout << h2 << std::endl;
  std::cout << h3 << std::endl;

  Segment<long double> s1(p,q), s2(p,t);
  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl << std::endl;

  Vector2<long double> pp(1,1);
  std::vector<Vector2<long double>> res = r1.getDistNeigh(pp, r1.getDistance(pp));
  for(int i=0; i<res.size(); i++)
    std::cout << res[i] << std::endl;
  std::cout << res.size() << std::endl;

  Rect<long double> rvert2(2,7);
  Vector2<long double> pq(1,1);
  std::vector<Vector2<long double>> res2 = rvert2.getDistNeigh(pq, 4);
  for(int i=0; i<res2.size(); i++)
    std::cout << res2[i] << std::endl;
  std::cout << res2.size() << std::endl;

  Rect<long double> r3(0,5);
  std::cout << r3 << std::endl;
  p.x = 5; p.y = 2;
  res2 = r3.getDistNeigh(p, 3);
  for(int i=0; i<res2.size(); i++)
    std::cout << res2[i] << std::endl;
  std::cout << res2.size() << std::endl;

  std::cout << r1.getDistance(pp) << std::endl;

  p.x = 0; p.y = 1;
  q.x = 4; q.y = -1;
  r3 = Rect<long double>(p,q);
  std::cout << r3 << std::endl << std::endl << std::endl;

  Segment<long double> seg1(0,0,2,0), seg2(1,-1,1,1);
  std::cout << seg1 << std::endl;
  std::cout << seg1.getRect() << std::endl;
  std::cout << seg2 << std::endl;
  std::cout << seg2.getRect() << std::endl;

  std::cout << "intersect? "<< seg1.intersect(seg2) << std::endl;

  std::cout << seg1.getRect().eval(Vector2<long double>(1,-1)) << std::endl;
  std::cout << seg1.getRect().eval(Vector2<long double>(1,1)) << std::endl;
  std::cout << seg2.getRect().eval(Vector2<long double>(0,0)) << std::endl;
  std::cout << seg2.getRect().eval(Vector2<long double>(2,0)) << std::endl << std::endl;

  Vector2<long double> a(0,0), b(1,1), c(0,1), d(1,0);

  Segment<long double> s3(a,b), s4(c,d);
  std::cout << s3.intersect(s4) << std::endl;

  Ball<long double> b1(a,std::sqrt(0.5)), b2(b,std::sqrt(0.5));
  std::cout << b1 << std::endl << b2 << std::endl;
  std::vector<Vector2<long double>> points = b1.intersections(b2);
  if(points.size()==0)
    std::cout << "no intersections" << std::endl;
  for(int i=0; i<points.size(); i++)
    std::cout << points[i] << std::endl;

  Ball<long double> b3(a,2), b4(c,1);
  points = b3.intersections(b4);
  std::cout << std::endl << b3 << std::endl << b4 << std::endl;
  if(points.size()==0)
    std::cout << "no intersections" << std::endl;
  for(int i=0; i<points.size(); i++)
    std::cout << points[i] << std::endl;
  
  std::cout << std::endl << std::endl << "Arcs checking" << std::endl;
  
  std::cout.precision(10);
  
  b1 = Ball<long double>(0,0,1);
  CircArc<long double> a1(b1, 0, -1, M_PI), a2(b1, 0, 1, 1.5*M_PI);
  std::cout << "a1: " << a1 << std::endl;
  std::cout << "a2: " << a2 << std::endl;
  std::vector<CircArc<long double>> resArc = a1.diff(a2);
  for(int i=0; i<resArc.size(); i++)
    std::cout << resArc[i] << std::endl;
  
  CircArc<long double> test(0,0,1);
  std::cout << "test: " << test << std::endl;
  std::cout << "test: " << test.rotate_arc(M_PI) << std::endl;
  std::cout << "test: " << test.rotate_arc(2*M_PI) << std::endl;
  
  Vector2<long double> v1(1,0), v2(0,1), v3(-1,0), v4(0,-1), v5(1,1), v6(-1,1), v7(-1,-1), v8(1,-1);
  std::cout << angle_vects(v1,v2) << std::endl;
  std::cout << angle_vects(v1,v3) << std::endl;
  std::cout << angle_vects(v1,v4) << std::endl;
  std::cout << angle_vects(v1,v1) << std::endl;
  std::cout << angle_vects(v1,v5) << std::endl;
  std::cout << angle_vects(v1,v6) << std::endl;
  std::cout << angle_vects(v1,v7) << std::endl;
  std::cout << angle_vects(v1,v8) << std::endl;
  std::cout << angle_vects(v5,v8) << std::endl;
  
  b1 = Ball<long double>(0,0,1);
  b2 = Ball<long double>(1,0,1);
  b3 = Ball<long double>(0,1,1);
  b4 = Ball<long double>(0,2,1);
  
  std::vector<Vector2<long double>> points2 = b1.intersections(b2);
  if(points2.size()==0)
    std::cout << "no intersections" << std::endl;
  for(int i=0; i<points2.size(); i++)
    std::cout << points2[i] << std::endl;
  
  points2 = b1.intersections(b3);
  if(points2.size()==0)
    std::cout << "no intersections" << std::endl;
  for(int i=0; i<points2.size(); i++)
    std::cout << points2[i] << std::endl;
  
  points2 = b2.intersections(b3);
  if(points2.size()==0)
    std::cout << "no intersections" << std::endl;
  for(int i=0; i<points2.size(); i++)
    std::cout << points2[i] << std::endl;
  
  std::cout << b1.intersection_arc(b2) << std::endl;
  std::cout << b1.intersection_arc(b3) << std::endl;
  std::cout << b2.intersection_arc(b3) << std::endl;
  std::cout << b1.intersection_arc(b4) << std::endl << std::endl << std::endl;
  
  a1 = CircArc<long double>(b1); // whole circumference
  b2 = Ball<long double>(1,0,1);
  b3 = Ball<long double>(-1,0,1);
  
  std::vector<CircArc<long double>> diff1 = a1.removeBall(b2);
  for(size_t i=0; i<diff1.size(); i++)
    std::cout << diff1[i] << std::endl;
  
  diff1 = diff1[0].removeBall(b3);
  for(size_t i=0; i<diff1.size(); i++)
    std::cout << diff1[i] << std::endl;
    
  diff1 = a1.removeBall(b1);
  if(diff1.size()==0)
    std::cout << "nothing remains" << std::endl;
  for(size_t i=0; i<diff1.size(); i++)
    std::cout << diff1[i] << std::endl;
  
  std::cout << "removing b4 from b1 circumfernce" << std::endl;
  diff1 = a1.removeBall(b4);
  if(diff1.size()==0)
    std::cout << "nothing remains" << std::endl;
  for(size_t i=0; i<diff1.size(); i++)
    std::cout << diff1[i] << std::endl;
  
  std::cout << "removing b3 from b1 circumfernce" << std::endl;
  std::cout << a1 << std::endl;
  std::cout << b3 << std::endl;
  diff1 = a1.removeBall(b3);
  if(diff1.size()==0)
    std::cout << "nothing remains" << std::endl;
  for(size_t i=0; i<diff1.size(); i++)
    std::cout << diff1[i] << std::endl;
  
  std::cout << std::endl << M_PI/4 << std::endl << std::endl;
  
  b1 = Ball<long double>(0,0,1);
  b2 = Ball<long double>(0,1,1);
  a1 = CircArc<long double>(b1); // whole circumference
  a2 = CircArc<long double>(b2); // whole circumference
  
  std::cout << "a1: " << a1 << std::endl;
  std::cout << "a2: " << a2 << std::endl;
  std::cout << "b3: " << b3 << std::endl;
  std::vector<CircArc<long double>> coll = collective_removeBall({a1,a2}, b3);
  std::cout << coll.size() << std::endl;
  for(size_t i=0; i<coll.size(); i++)
    std::cout << coll[i] << std::endl;
  
  
  std::cout << "bal containing ball" << std::endl;
  b1 = Ball<long double>(0,0,1);
  b2 = Ball<long double>(0,1,2);
  b3 = Ball<long double>(0,0,9);
  b4 = Ball<long double>(0,0,0.1);
  a1 = CircArc<long double>(b1); // whole circumference
  a2 = CircArc<long double>(b2); // whole circumference
  
  std::cout << "b2 contain b1? " << b2.containsBall(b1) << std::endl;
  std::cout << "b1 contain b2? " << b1.containsBall(b2) << std::endl;
  std::cout << "b1 contain b1? " << b1.containsBall(b1) << std::endl;
  
  coll = collective_removeBall({a1,a2}, b3);
  std::cout << coll.size() << std::endl;
  coll = collective_removeBall({a1,a2}, b4);
  std::cout << coll.size() << std::endl;
  std::cout << coll[0] << std::endl;
  std::cout << coll[1] << std::endl;
}

/*** R
testRcpp()
*/
