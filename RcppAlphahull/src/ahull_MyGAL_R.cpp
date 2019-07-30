#include <Rcpp.h>
#include <cmath>
#include "newClasses/Rect.h"
#include "newClasses/Ball.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

template<typename T>
using complement_type = std::pair<std::vector<Ball<T>>, std::vector<HalfPlane<T>>>;

template<typename T>
complement_type<T> computeComplement(const Rcpp::NumericMatrix& mesh, const T& alpha){
  std::vector<Ball<T>> balls;
  std::vector<HalfPlane<T>> halfplanes;
  balls.reserve(4*mesh.rows()); // reserving memory for the worst case
  halfplanes.reserve(mesh.rows()); // reserving memory for the worst case

  for(int i=0; i<mesh.rows(); i++){
    // computing rects
    Vector2<T> p(mesh(i,2), mesh(i,3));
    Vector2<T> q(mesh(i,4), mesh(i,5));
    Vector2<T> e1(mesh(i,6), mesh(i,7));
    Vector2<T> e2(mesh(i,8), mesh(i,9));

    Rect<T> r(p,q), bis(e1,e2);

    std::vector<Vector2<T>> res = bis.getDistNeigh(p,alpha);
    // in this case i only add two balls or a ball and an halfplane
    if(res.size() == 0){
      if(mesh(i,10)==1) // bp1 == 1 => halfplane
        halfplanes.push_back(HalfPlane<T>(r, r.eval(e1)==1? true: false )); // add the new halfplane
        // the side of the halfplane is determined by the position of the point e1 wrt the rect r
        // NB: by how is contructed the matrix mesh it can't happen that the point lies on the boundary
      else
        balls.push_back(Ball<T>(e1, p.getDistance(e1))); // add the new ball

      if(mesh(i,11)==1) // bp2 == 1 => halfplane
        halfplanes.push_back(HalfPlane<T>(r, r.eval(e2)==1? true: false )); // add the new halfplane
      else
        balls.push_back(Ball<T>(e2, p.getDistance(e2))); // add the new ball
    }

    // check how many balls and halfplanes I have to add
  }

  complement_type<T> complement;
  complement.first = balls;
  complement.second = halfplanes;

  return complement;
};

/* Function to retrieve a ahull object (almost) like the one returned by the alphahull function ahull,
 * but emplying previous results from the c++ library MyGAL
 */
// [[Rcpp::export]]
Rcpp::List computeAhullRcpp(Rcpp::List ashape) {
  // Retrieve the complement of the alpha-hull
  Rcpp::NumericMatrix mesh = as<Rcpp::List>(ashape["delvor.obj"])["mesh"];
  const long double alpha = ashape["alpha"];

  // the complement is a union of open balls and open halfplanes
  complement_type<long double> complement = computeComplement<long double>(mesh, alpha);

  Rcpp::List ahull = Rcpp::List::create(Rcpp::Named("arcs") = 1,
                                        Rcpp::Named("xahull") = 1,
                                        Rcpp::Named("complement") = 1,
                                        Rcpp::Named("length") = 1,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  return ahull;
}
