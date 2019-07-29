#include <Rcpp.h>
#include <cmath>
#include "newClasses/Segment.h"
#include "newClasses/Rect.h"
#include "newClasses/Ball.h"
#include "newClasses/HalfPlane.h"
using namespace Rcpp;

// [[Rcpp::export]]
void test() {
  Rect<double> r1(2,7), rvert(2.457489485);
  Vector2<double> p(1,9), q(1/3, 2/3+7), t(2.457489485, 393);
  Vector2<double> u(1/11, r1.eval(1/11));

  Rcpp::Rcout << r1.isVertical() << std::endl;
  Rcpp::Rcout << rvert.isVertical() << std::endl;

  Rcpp::Rcout << r1.eval(p) << std::endl;
  Rcpp::Rcout << r1.eval(q) << std::endl;
  Rcpp::Rcout << r1.eval(t) << std::endl;
  Rcpp::Rcout << r1.eval(u) << std::endl;

  Rcpp::Rcout << rvert.eval(p) << std::endl;
  Rcpp::Rcout << rvert.eval(q) << std::endl;
  Rcpp::Rcout << rvert.eval(t) << std::endl;

  HalfPlane<double> h1(r1), h2(r1, false), h3(rvert);

  std::cout << r1 << std::endl;
  std::cout << rvert << std::endl;
  std::cout << h1 << std::endl;
  std::cout << h2 << std::endl;
  std::cout << h3 << std::endl;

  Segment<double> s1(p,q), s2(p,t);
  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl << std::endl;

  Vector2<double> pp(1,1);
  std::vector<Vector2<double>> res = r1.getDistNeigh(pp, std::sqrt(37));
  for(int i=0; i<res.size(); i++)
    std::cout << res[i] << std::endl;
  std::cout << res.size() << std::endl;

  Rect<double> rvert2(2,7);
  Vector2<double> pq(1,1);
  std::vector<Vector2<double>> res2 = rvert2.getDistNeigh(pq, 4);
  for(int i=0; i<res2.size(); i++)
    std::cout << res2[i] << std::endl;
  std::cout << res2.size() << std::endl;

  Rect<double> r3(0,5);
  std::cout << r3 << std::endl;
  p.x = 5; p.y = 2;
  res2 = r3.getDistNeigh(p, 3);
  for(int i=0; i<res2.size(); i++)
    std::cout << res2[i] << std::endl;
  std::cout << res2.size() << std::endl;

  std::cout << r1.getDistance(pp);
}

/*** R
  test()
*/
