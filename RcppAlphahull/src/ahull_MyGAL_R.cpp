#include <Rcpp.h>
#include <cmath>
#include "newClasses/Ball.h"
#include "newClasses/Rect.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "newClasses/CircArc.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

Rcpp::NumericMatrix computeComplement(const Rcpp::NumericMatrix& mesh, const long double& alpha);

// starting from the complement matrix of the alpha hull returns a matrix containing arcs that describe
// the boundary
template<typename T>
Rcpp::NumericMatrix getArcs(const Rcpp::NumericMatrix& complement){
  std::vector<Ball<T>> balls;
  std::vector<HalfPlane<T>> halfplanes;
  
  // constructing balls, halfplanes and arcs that has to be "reduced"
  for(int i=0; i<complement.rows(); i++)
    if(complement(i,2)>0){ // r > 0 => ball
      Ball<T> b(complement(i,0), complement(i,1), complement(i,2));
      // it may happen that some balls are inserted more than one time, in those cases I just insert one
      if( std::find(balls.begin(), balls.end(), b)==balls.end() )
        balls.push_back(b);
    }
    else{
      bool side = complement(i,2) == -1 || complement(i,2) == -3? true: false; // halfplane has form with ">"
      if( complement(i,0) > -3 ) // in case r = -1 or r = -2 (non vertical halfplane)
        halfplanes.push_back(HalfPlane<T>(complement(i,1),complement(i,0),side));
      else
        halfplanes.push_back(HalfPlane<T>(complement(i,0),side));
    }
    
  std::vector<CircArc<T>> arcs = union_boundary(balls); // construct the boundary of the union of balls
  std::list<CircArc<T>> arcs_list(arcs.begin(), arcs.end());
  
  // clamping arcs that are outside the convex hull (namely are in the halfplanes). By construction such
  // arcs have their starting and ending point at most on the boundary of one halfplane so I check if
  // the middle point is in any of them.
  for(size_t i=0; i<halfplanes.size(); i++)
    arcs_list.remove_if([&](const CircArc<T>& a){ return halfplanes[i].isIn(a.getMidPoint()); });
  
  arcs = std::vector<CircArc<T>>(arcs_list.begin(), arcs_list.end());
  
  // building arcs matrix
  Rcpp::NumericMatrix arcs_mat(arcs.size(), 8);
  for(size_t i=0; i<arcs.size(); i++){
    Vector2<T> v = arcs[i].getMidVector(); // retrieving this vector in order to match alphahull description of arcs
    arcs_mat(i,0) = arcs[i].getBall().center().x;
    arcs_mat(i,1) = arcs[i].getBall().center().y;
    arcs_mat(i,2) = arcs[i].getBall().radius();
    arcs_mat(i,3) = v.x;
    arcs_mat(i,4) = v.y;
    arcs_mat(i,5) = arcs[i].width()/2;
  }
  colnames(arcs_mat) = Rcpp::CharacterVector::create("c1", "c2", "r", "vx", "vy", "theta", "end1", "end2");
  
  return arcs_mat;
}

// given the arcs that form the boundary of the alpha-hull return its length
template<typename T>
T ahull_boundary_length(const Rcpp::NumericMatrix& arcs){
  T length = 0;
  for(int i=0; i<arcs.rows(); i++)
    if(arcs(i,2)>0)
      length+=2*arcs(i,5)*arcs(i,2); // 2*theta*r
  return length;
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

  // arcs of the boundary of the alpha-hull
  Rcpp::NumericMatrix arcs = getArcs<real>(complement);
  
  // computing length of the alpha hull boundary
  real length = ahull_boundary_length<real>(arcs);
  
  Rcpp::List ahull = Rcpp::List::create(Rcpp::Named("arcs") = arcs,
                                        Rcpp::Named("xahull") = 1, // seems unused in alphahull functions (I'm seriously thinking of removing it)
                                        Rcpp::Named("length") = length,
                                        Rcpp::Named("complement") = complement,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  ahull.attr("class") = "ahull";

  return ahull;
}
