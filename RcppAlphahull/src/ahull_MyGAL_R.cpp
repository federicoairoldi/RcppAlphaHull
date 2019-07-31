#include <Rcpp.h>
#include <cmath>
#include "utilities.h"
#include "newClasses/Rect.h"
#include "newClasses/Ball.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

/* Function to retrieve a ahull object (almost) like the one returned by the alphahull function ahull,
 * but emplying previous results from the c++ library MyGAL
 */
// [[Rcpp::export]]
Rcpp::List computeAhullRcpp(Rcpp::List ashape) {
  // choosing the floating point representation to be used
  typedef long double real;

  // Retrieve the complement of the alpha-hull
  Rcpp::NumericMatrix mesh = as<Rcpp::List>(ashape["delvor.obj"])["mesh"];
  const real alpha = ashape["alpha"];

  // the complement is a union of open balls and open halfplanes
  Rcpp::NumericMatrix complement = util::computeComplement<real>(mesh, alpha);

  Rcpp::List ahull = Rcpp::List::create(Rcpp::Named("arcs") = 1,
                                        Rcpp::Named("xahull") = 1,
                                        Rcpp::Named("complement") = complement,
                                        Rcpp::Named("length") = 1,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  ahull.attr("class") = "ahull";

  return ahull;
}
