#include <Rcpp.h>
#include <iostream>
#include <algorithm>
#include "MyGAL/FortuneAlgorithm.h"
#include "newClasses/Segment.h"

using namespace mygal;
using namespace Rcpp;

/* Returns the alpha-limits for the sites:
 * one can show that for each site of the diagram it holds
 * p=(x;y) alpha-extreme for any alpha <= alpha^i_L
 * alpha^i_L = max( dist(p,v), v vertex of face of point p in the voronoi diagram )
 */
std::vector<long double> computeAlphaLimits(const Rcpp::List& delvor_obj){
   std::vector<long double> alpha_L((int) as<Rcpp::List>(delvor_obj["tri.obj"])["n"], 0);
   const Rcpp::NumericMatrix& mesh = delvor_obj["mesh"];
   long double dist1, dist2, tmp;
   for(int i=0; i<mesh.rows(); i++){
      size_t idx1 = (mesh(i,0)-1), idx2 = (mesh(i,1)-1);
      if(mesh(i,10) == 1 || mesh(i,11) == 1){
         alpha_L[idx1] = R_PosInf;
         alpha_L[idx2] = R_PosInf;
      }
      else{
         Vector2<long double> p(mesh(i,2), mesh(i,3));
         Vector2<long double> e1(mesh(i,6), mesh(i,7));
         Vector2<long double> e2(mesh(i,8), mesh(i,9));

         dist1 = p.getDistance(e1);
         dist2 = p.getDistance(e2);
         tmp = std::max(dist1, dist2);
         alpha_L[idx1] = std::max(alpha_L[idx1], tmp);
         alpha_L[idx2] = std::max(alpha_L[idx2], tmp);
      }
   }

   return alpha_L;
}

// Computes the length of the alpha-shape
template<typename T>
T ashape_length(const Rcpp::NumericMatrix& mesh){
   T length = 0;
   for(int i=0; i<mesh.rows(); i++){
      Vector2<T> p1(mesh(i,2), mesh(i,3));
      Vector2<T> p2(mesh(i,4), mesh(i,5));
      length+=p1.getDistance(p2);
   }
   return length;
}

/* Returns which rows of the matrix mesh are part of the alpha hull for the given value of alpha (recall
 * that the alpha-shape is a subset of the Delanuay triangulation)
 */
std::vector<size_t> getAlphaNeighbours(const Rcpp::NumericMatrix& mesh,
                                       const std::vector<size_t>& alpha_extremes,
                                       const double& alpha){
   std::vector<size_t> which_rows;
   long double alpha_min, alpha_max, dist1, dist2, dist3;
   for(int i=0; i<mesh.rows(); i++)
      if(std::find(alpha_extremes.begin(), alpha_extremes.end(), mesh(i,0)) != alpha_extremes.end()
         && std::find(alpha_extremes.begin(), alpha_extremes.end(), mesh(i,1)) != alpha_extremes.end()){
         // checking alpha_min e alpha_max
         Vector2<long double> p(mesh(i,2), mesh(i,3));
         Vector2<long double> q(mesh(i,4), mesh(i,5));
         Vector2<long double> e1(mesh(i,6), mesh(i,7));
         Vector2<long double> e2(mesh(i,8), mesh(i,9));

         dist1 = (mesh(i,10) == 1? R_PosInf: p.getDistance(e1));
         dist2 = (mesh(i,11) == 1? R_PosInf: p.getDistance(e2));
         dist3 = p.getDistance(q);

         Segment<long double> s1(p,q), s2(e1, e2);

         alpha_min = (s1.intersect(s2)? dist3/2: std::min(dist1, dist2));
         alpha_max = std::max(dist1, dist2);

         if(alpha_min <= alpha && alpha <= alpha_max)
            which_rows.push_back(i);
      }

      return which_rows;
}

// [[Rcpp::export]]
Rcpp::List computeAshapeRcpp(const Rcpp::List& delvor_obj, const long double& alpha) {
  /* delvor_obj has to be a "delvor" object and therefore is a list with the following components:
   * - mesh: matrix containing information about the voronoi tesselation and delanuay
   *   triangulation relative to the sites contained in the 2nd element of the list
   * - x: matrix nx2 containing the coordinates of the sites
   * - tri.obj: a tri object or a fake.tri object containing information regarding
   *            the delanuay triangulation
   */

  /* Find the "alpha-limits" for the sites:
   * one can show that for each site of the diagram it holds
   * p=(x;y) alpha-extreme for any alpha <= alpha^i_L
   * alpha^i_L = max( dist(p,v), v vertex of face of point p in the voronoi diagram )
   */
   std::vector<long double> alpha_L = computeAlphaLimits(delvor_obj);

   // Select the alpha-extreme sites
   std::vector<size_t> alpha_extremes;
   for(int i=0; i<(int) as<Rcpp::List>(delvor_obj["tri.obj"])["n"]; i++)
        if(alpha <= alpha_L[i])
           alpha_extremes.push_back(i+1); // may be quite slow (remember to check it)

   // Select the alpha-neighbours rows of the matrix mesh
   std::vector<size_t> which_rows = getAlphaNeighbours(delvor_obj["mesh"], alpha_extremes, alpha);

   // Extracting the needed rows
   Rcpp::NumericMatrix mesh = delvor_obj["mesh"];
   Rcpp::NumericMatrix edges(which_rows.size(), mesh.cols());
   colnames(edges) = colnames(mesh);
   for(int i=0; i<which_rows.size(); i++)
      edges(i,_) = mesh(which_rows[i],_);

   Rcpp::List ashape = Rcpp::List::create(Rcpp::Named("edges") = edges,
                                          Rcpp::Named("length") = ashape_length<long double>(edges),
                                          Rcpp::Named("alpha") = alpha,
                                          Rcpp::Named("alpha.extremes") = alpha_extremes,
                                          Rcpp::Named("delvor.obj") = delvor_obj,
                                          Rcpp::Named("x") = delvor_obj["x"]);
   ashape.attr("class") = "ashape";

   return ashape;
}

