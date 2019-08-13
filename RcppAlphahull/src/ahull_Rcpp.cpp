#include <Rcpp.h>
#include <cmath>
#include "newClasses/Ball.h"
#include "newClasses/Line.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "newClasses/CircArc.h"
#include "MyGAL/Vector2.h"
#include "utilities.h"
using namespace Rcpp;

Rcpp::NumericMatrix computeComplement(const Rcpp::NumericMatrix& mesh, const long double& alpha);

// starting from the complement matrix of the alpha hull returns a matrix containing arcs that describe
// the boundary
template<typename T>
Rcpp::NumericMatrix getArcs(const Rcpp::NumericMatrix& complement){
  std::vector<Ball<T>> balls;
  std::vector<HalfPlane<T>> halfplanes;
  
  complement_matrix_to_vectors(complement, balls, halfplanes);
  
  std::vector<CircArc<T>> arcs = union_boundary(balls); // construct the boundary of the union of balls
  std::list<CircArc<T>> arcs_list(arcs.begin(), arcs.end()); // inserting in a list since it's easier to remove elements in the middle
  
  // clamping arcs that are outside the convex hull (namely are in the halfplanes). By construction such
  // arcs have their starting and ending point at most on the boundary of one halfplane so I check if
  // the middle point is in any of them.
  for(typename std::vector<HalfPlane<T>>::const_iterator hp_it=halfplanes.cbegin(); hp_it!=halfplanes.cend(); hp_it++)
    arcs_list.remove_if([&](const CircArc<T>& a){ return hp_it->isIn(a.getMidPoint()); });
  
  /*
  for(size_t i=0; i<halfplanes.size(); i++){
    std::cout << std::endl << "currente hp: " << halfplanes[i] << std::endl;
    for(auto it = arcs_list.begin(); it!=arcs_list.end(); it++){
      std::cout << "mid point: " << it->getMidPoint() << " is in?" << halfplanes[i].isIn(it->getMidPoint()) << std::endl;
      if(halfplanes[i].isIn(it->getMidPoint())){
        it = arcs_list.erase(it);
        it--;
      }
    }
  }*/
  
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
                                        //Rcpp::Named("xahull") = 1, // seems unused in alphahull functions (I'm seriously thinking of removing it)
                                        Rcpp::Named("length") = length,
                                        Rcpp::Named("complement") = complement,
                                        Rcpp::Named("alpha") = ashape["alpha"],
                                        Rcpp::Named("ashape.obj") = ashape);
  ahull.attr("class") = "ahull";

  return ahull;
}
