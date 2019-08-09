#include <Rcpp.h>
#include <iostream>
#include <algorithm>
#include "newClasses/Segment.h"
#include "utilities.h"

using namespace mygal;
using namespace Rcpp;

/* Returns the alpha-limits for the sites:
 * one can show that for each site of the diagram it holds
 * p=(x;y) alpha-extreme for any alpha <= alpha^i_L
 * alpha^i_L = max( dist(p,v), v vertex of face of point p in the voronoi diagram )
 */
template<typename T>
std::vector<T> computeAlphaLimits(const Rcpp::List& delvor_obj){
   std::vector<T> alpha_L((int) as<Rcpp::List>(delvor_obj["tri.obj"])["n"], 0); // vector to store the limit values
   const Rcpp::NumericMatrix& mesh = delvor_obj["mesh"];
   for(int i=0; i<mesh.rows(); i++){
      size_t idx1 = (mesh(i,0)-1), idx2 = (mesh(i,1)-1); // retrieving 1st and 2nd site indices
      if(mesh(i,10) == 1 || mesh(i,11) == 1){ 
         // if the edge is infinite then the sites are alpha-extreme for any alpha, for simplicity I set
         // to infinite their limit values
         alpha_L[idx1] = std::numeric_limits<T>::infinity();
         alpha_L[idx2] = std::numeric_limits<T>::infinity();
      }
      else{
         Vector2<T>  p(mesh(i,2), mesh(i,3)); // 1st site
         Vector2<T> e1(mesh(i,6), mesh(i,7)); // 1st endpoint of the current Voronoi edge
         Vector2<T> e2(mesh(i,8), mesh(i,9)); // 2nd endpoint of the current Voronoi edge

         T dist1 = p.getDistance(e1); // distance from p to e1
         T dist2 = p.getDistance(e2); // distance from p to e2
         T tmp = std::max(dist1, dist2);
         alpha_L[idx1] = std::max(alpha_L[idx1], tmp); // updating the limit value for the 1st site
         alpha_L[idx2] = std::max(alpha_L[idx2], tmp); // updating the limit value for the 2nd site
      }
   }

   return alpha_L;
}

// Computes the length of the alpha-shape given the mesh matrix describing it
template<typename T>
T ashape_length(const Rcpp::NumericMatrix& mesh){
   T length = 0;
   for(int i=0; i<mesh.rows(); i++){
      Vector2<T> p(mesh(i,2), mesh(i,3));
      Vector2<T> q(mesh(i,4), mesh(i,5));
      length+=p.getDistance(q);
   }
   return length;
}

/* Returns which rows of the matrix mesh are part of the alpha hull for the given value of alpha (recall
 * that the alpha-shape is a subset of the Delaunay triangulation)
 * 
 * T -> precision of floating point
 * T2 -> type for the indices (size_t or int)
 * 
 */
template<typename T, typename T2>
std::vector<T2> getAlphaNeighbours(const Rcpp::NumericMatrix& mesh,
                                   const std::vector<T2>& alpha_extremes,
                                   const T& alpha){
   std::vector<T2> which_rows;
   for(int i=0; i<mesh.rows(); i++)
      if(std::find(alpha_extremes.begin(), alpha_extremes.end(), mesh(i,0)) != alpha_extremes.end()
         && std::find(alpha_extremes.begin(), alpha_extremes.end(), mesh(i,1)) != alpha_extremes.end()){
         // checking alpha_min e alpha_max
         Vector2<T>  p(mesh(i,2), mesh(i,3));
         Vector2<T>  q(mesh(i,4), mesh(i,5));
         Vector2<T> e1(mesh(i,6), mesh(i,7));
         Vector2<T> e2(mesh(i,8), mesh(i,9));

         T dist1 = (mesh(i,10) == 1? std::numeric_limits<T>::infinity(): p.getDistance(e1));
         T dist2 = (mesh(i,11) == 1? std::numeric_limits<T>::infinity(): p.getDistance(e2));
         T dist3 = p.getDistance(q);

         Segment<T> s1(p,q), s2(e1, e2);

         T alpha_min = (s1.intersect(s2)? dist3/2: std::min(dist1, dist2));
         T alpha_max = std::max(dist1, dist2);

         if(alpha_min <= alpha && alpha <= alpha_max)
            which_rows.push_back(i);
      }

   return which_rows;
}

// [[Rcpp::export(".computeAshapeRcpp")]]
Rcpp::List computeAshapeRcpp(const Rcpp::List& delvor_obj, const long double& alpha) {
   // choosing the floating point representation to be used
   typedef long double real;
   
  /* delvor_obj has to be a "delvor" object and therefore is a list with the following components:
   * - mesh: matrix containing information about the voronoi tesselation and Delaunay
   *   triangulation relative to the sites contained in the 2nd element of the list
   * - x: matrix nx2 containing the coordinates of the sites
   * - tri.obj: a tri object or a tri.mod object containing information regarding
   *            the Delaunay triangulation
   */

  /* Find the "alpha-limits" for the sites:
   * one can show that for each site of the diagram it holds
   * p=(x;y) alpha-extreme for any alpha <= alpha^i_L
   * alpha^i_L = max( dist(p,v), v vertex of face of point p in the voronoi diagram )
   */
   std::vector<real> alpha_L = computeAlphaLimits<real>(delvor_obj);

   // Select the alpha-extreme sites
   std::vector<size_t> alpha_extremes;
   for(size_t i=0; i<(size_t) as<Rcpp::List>(delvor_obj["tri.obj"])["n"]; i++)
        if(alpha <= alpha_L[i])
           alpha_extremes.push_back(i+1); // may be quite slow (remember to check it)
        
   // Select the alpha-neighbours rows of the matrix mesh
   std::vector<size_t> which_rows = getAlphaNeighbours<real>(delvor_obj["mesh"], alpha_extremes, alpha);

   // Extracting the needed rows
   Rcpp::NumericMatrix mesh = delvor_obj["mesh"];
   Rcpp::NumericMatrix edges(which_rows.size(), mesh.cols());
   colnames(edges) = colnames(mesh);
   for(size_t i=0; i<which_rows.size(); i++)
      edges(i,_) = mesh(which_rows[i],_);

   Rcpp::List ashape = Rcpp::List::create(Rcpp::Named("edges") = edges,
                                          Rcpp::Named("length") = ashape_length<real>(edges),
                                          Rcpp::Named("alpha") = alpha,
                                          Rcpp::Named("alpha.extremes") = as<Rcpp::IntegerVector>(alpha_extremes),
                                          Rcpp::Named("delvor.obj") = delvor_obj,
                                          Rcpp::Named("x") = delvor_obj["x"]);
   ashape.attr("class") = "ashape";

   return ashape;
}

