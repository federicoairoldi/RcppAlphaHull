#include <Rcpp.h>
#include <cmath.h>
#include "newClasses/Segment.h"
#include "newClasses/Rect.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

/*
 * Function to retrieve a ahull object (almost) like the one returned by the alphahull function ahull, but
 * emplying previous results the c++ library MyGAL
 */
// [[Rcpp::export]]
Rcpp::List computeAhullRcpp(Rcpp::List ashape) {
  // Retrieve the complement of the alpha-hull
  Rcpp::NumericMatrix mesh = as<Rcpp::List>(ashape["delvor.obj"])["mesh"];
  const long double alpha = ashape["alpha"];

  for(int i=0; i<mesh.rows(); i++){
    // computing rects
    Vector2<long double> p(mesh(i,2), mesh(i,3));
    Vector2<long double> q(mesh(i,4), mesh(i,5));
    Vector2<long double> e1(mesh(i,6), mesh(i,7));
    Vector2<long double> e2(mesh(i,8), mesh(i,9));

    Segment<long double> s1(p,q), s2(e1, e2);

    Rect<long double> r1 = s1.getRect(), r2 = s2.getRect();

    long double val1 = r1.eval(e1),
                val2 = r1.eval(e2);
                //delta = (1+std::pow(m1, 2))*std::pow(alpha, 2)-std::pow(q1-p.y-m1*p.x, 2);

  }

  Rcpp::List ahull = Rcpp::List::create(Rcpp::Named("arcs") = 1,
                                        Rcpp::Named("xahull") = 1,
                                        Rcpp::Named("complement") = 1,
                                        Rcpp::Named("length") = 1,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  return ahull;
}
