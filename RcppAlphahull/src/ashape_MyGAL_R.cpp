#include <Rcpp.h>
#include <iostream>
#include <algorithm>
#include "MyGAL/FortuneAlgorithm.h"

using namespace mygal;
using namespace Rcpp;

/* Returns the alpha-limits points:
 * one can show that for each site of the diagram it holds
 * p=(x;y) alpha-extreme for any alpha <= alpha^i_L
 * alpha^i_L = max( dist(p,v), v vertex of face of point p in the voronoi diagram )
 */
std::vector<long double> computeAlphaLimits(const Rcpp::List& delvor_obj){
   std::vector<long double> alpha_L((int) as<Rcpp::List>(delvor_obj["tri.obj"])["n"], 0);
   const Rcpp::NumericMatrix& mesh = delvor_obj["mesh"];
   long double dist1, dist2, tmp;
   for(int i=0; i<mesh.rows(); i++)
      if(mesh(i,10) == 1 || mesh(i,11) == 1){
         alpha_L[mesh(i,0)-1] = R_PosInf;
         alpha_L[mesh(i,1)-1] = R_PosInf;
      }
      else{
         dist1 = (Vector2<long double>(mesh(i,2), mesh(i,3))).getDistance(Vector2<long double>(mesh(i,6),mesh(i,7)));
         dist2 = (Vector2<long double>(mesh(i,2), mesh(i,3))).getDistance(Vector2<long double>(mesh(i,8),mesh(i,9)));
         tmp = std::max(dist1, dist2);
         alpha_L[mesh(i,0)-1] = std::max(alpha_L[mesh(i,0)-1], tmp);
         alpha_L[mesh(i,1)-1] = std::max(alpha_L[mesh(i,1)-1], tmp);
      }

   return alpha_L;
}

// [[Rcpp::export]]
Rcpp::List computeAshapeRcpp(const Rcpp::List& delvor_obj, const long double& alpha) {
  /* delvor_obj has to be a "delvor" object and therefore has to be a list with
   * the following components:
   * - mesh: matrix containing information about the voronoi tesselation and delanuay
   *   triangulation relative to the sites contained in the 2nd element of the list
   * - x: matrix nx2 containing the coordinates of the sites
   * - tri.obj: a tri object or a fake.tri object containing information regarding
   *            the delanuay triangulation
   */

  std::vector<long double> alpha_L = computeAlphaLimits(delvor_obj);

  Rcpp::IntegerVector alpha_extremes;
  for(int i=0; i<(int) as<Rcpp::List>(delvor_obj["tri.obj"])["n"]; i++)
     if(alpha <= alpha_L[i])
        alpha_extremes.push_back(i+1); // may be quite slow (remember to check it)

  Rcpp::List ashape = Rcpp::List::create(Rcpp::Named("edges") = 1,
                                         Rcpp::Named("length") = 1,
                                         Rcpp::Named("alpha") = alpha,
                                         Rcpp::Named("alpha.extremes") = alpha_extremes,
                                         Rcpp::Named("delvor.obj") = delvor_obj,
                                         Rcpp::Named("x") = delvor_obj["x"]);
   ashape.attr("class") = "ashape";

  return ashape;
}

