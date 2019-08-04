#include <Rcpp.h>
#include <cmath>
#include "newClasses/Rect.h"
#include "newClasses/Ball.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "newClasses/Arc.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

Rcpp::NumericMatrix computeComplement(const Rcpp::NumericMatrix& mesh, const long double& alpha);

// reduces the arcs by substracting those part that are inside the given balls
template<typename T>
std::vector<Arc<T>> reduce(const Arc<T>& arc, const std::vector<Ball<T>>& balls){
  std::vector<Arc<T>> arc_res{arc};
  Ball<T> ball = arc.getBall(); // ball on which pieces of arcs lays
  
  for(size_t i=0; i<balls.size(); i++){
    // check if the i-th ball intersect tha ball of the arc (if not I have nothing to do)
    std::vector<Vector2<T>> points = balls[i].intersections(ball);
    if(points.size()==2){
      //Arc<T>
    }
  }
}

// starting from the complement matrix of the alpha hull returns a matrix containing arcs that describe
// the boundary
template<typename T>
Rcpp::NumericMatrix getArcs(const Rcpp::NumericMatrix& complement, const T& alpha){
  std::vector<Arc<T>> arcs;
  std::vector<Ball<T>> balls;
  std::vector<HalfPlane<T>> halfplanes;
  
  // constructing balls, halfplanes and arcs that has to be "reduced"
  for(size_t i=0; i<complement.rows(); i++)
    if(complement(i,2)>0){ // r > 0 => ball
      Ball<T> b(complement(i,0), complement(i,1), complement(i,2));
      // it may happen that some balls are inserted more than one time, in those cases I just insert one
      if( std::find(balls.begin(), balls.end(), b)==balls.end() ){
        balls.push_back(b);
        arcs.push_back(Arc<T>(b));
      }
    }
    else{
      bool side = complement(i,2) == -1 || complement(i,2) == -3? true: false; // halfplane has form with ">"
      if( complement(i,0) > -3 ) // in case r = -1 or r = -2 (non vertical halfplane)
        halfplanes.push_back(HalfPlane<T>(complement(i,1),complement(i,0),side));
      else
        halfplanes.push_back(HalfPlane<T>(complement(i,0),side));
    }
  
  // cutting the arcs
  for(size_t i=0; i<arcs.size(); i++){
    std::vector<Arc<T>> tmp = reduce(arcs[i], balls);
  }
  
  Rcpp::NumericMatrix arcs_mat;
  return arcs_mat;
}

/* Function to retrieve a ahull object (almost) like the one returned by the alphahull function ahull,
 * but emplying previous results from the c++ library MyGAL
 */
// [[Rcpp::export(".computeAhullRcpp")]]
Rcpp::List computeAhullRcpp(Rcpp::List ashape) {
  // choosing the floating point representation to be used
  typedef long double real;

  // Retrieve the complement of the alpha-hull
  Rcpp::NumericMatrix mesh = as<Rcpp::List>(ashape["delvor.obj"])["mesh"];
  const real alpha = ashape["alpha"];

  // the complement is a union of open balls and open halfplanes
  Rcpp::NumericMatrix complement = computeComplement(mesh, alpha);

  // arcs
  Rcpp::NumericMatrix arcs;
  
  // computing length of the alpha hull boundary
  real length = 0;
  for(size_t i=0; i<arcs.rows(); i++)
    if(arcs(i,2)>0)
      length+=2*arcs(i,5)*arcs(i,2); // 2*theta*r
  
  Rcpp::List ahull = Rcpp::List::create(Rcpp::Named("arcs") = arcs,
                                        Rcpp::Named("xahull") = 1,
                                        Rcpp::Named("length") = length,
                                        Rcpp::Named("complement") = complement,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  ahull.attr("class") = "ahull";

  return ahull;
}
