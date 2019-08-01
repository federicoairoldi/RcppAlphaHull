#include <Rcpp.h>
#include <iostream>
#include <cmath>
#include "MyGAL/FortuneAlgorithm.h"

using namespace mygal;
using namespace Rcpp;

// NB: TRY TO MOVE IT TO UTILITIES.H //
/* Function to translate indices: since in c++ vectors starts from 0 and in R from 1,
 * then indeces of the sites are shifted by a -1 in c++ with respect the
 * correspondent in R
 */
template<typename T>
std::vector<T> add_one(const std::vector<T>& v){
  std::vector<T> newvett = v;
  for(size_t i=0; i<newvett.size(); i++)
    newvett[i]+=1;
  return newvett;
}

// casts a vector of size_t ro a vector of int
Rcpp::IntegerVector to_RcppInt(const std::vector<size_t>& v){
  Rcpp::IntegerVector newvett(v.size());
  for(size_t i=0; i<v.size(); i++)
    newvett[i]=v[i];
  return newvett;
}

// Returns if the point "point" is at the boundary of the box "box"
template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
  T eps(10e-5);
  if( ( std::fabs((point.x-box.left)/box.left) < eps | std::fabs((point.x-box.right)/box.right) < eps  )
        && point.y <= box.top && point.y >= box.bottom )
        return true;
  if( ( std::fabs((point.y-box.top)/box.top) < eps | std::fabs((point.y-box.bottom)/box.bottom) < eps  )
        && point.x <= box.right && point.x >= box.left )
        return true;
  return false;
}

/*
 * Function to retrieve a delvor object (almost) like the one returned by the alphahull function delvor,
 * but using the c++ library MyGAL
 */
// [[Rcpp::export(".computeVoronoiRcpp")]]
Rcpp::List computeVoronoiRcpp(const Rcpp::NumericVector x, const Rcpp::NumericVector y) {
  // choosing the floating point representation to be used
  typedef long double real;

  // useless if performed by xy.coord in R
  if( x.size() != y.size() )
    Rcpp::stop("Error! x and y vectors don't have same length!");

  // Build the set of sites
  std::vector<Vector2<real>> points;
  for(size_t i=0; i<x.size(); i++)
    points.push_back(Vector2<real>(x[i], y[i]));

  // Call of the Fortune's algorithm to build the tesselation and the triangulation:
  // procedure suggested by the author of the library MyGAL
  auto algorithm = FortuneAlgorithm<real>(points); // initialize an instance of Fortune's algorithm
  algorithm.construct();                            // construct the diagram

  Box<real> localbox{0, 0, 1, 1};
  algorithm.bound(&localbox);            // Bound the diagram
  auto diagram = algorithm.getDiagram(); // Get the constructed diagram
  //diagram.intersect(Box<ftype>{0, 0, 2, 2}); // Compute the intersection between the diagram and a box
  auto triangulation = diagram.computeTriangulation();

  // Extract information and generate the R object
  // 1. Construct the matrix of coordinates
  Rcpp::NumericMatrix coord(x.size(), 2);
  coord(_, 0) = x;
  coord(_, 1) = y;

  // 2. Construct the matrix mesh (contains delanuay/voronoi information)
  auto halfedges = diagram.getHalfEdges();

  // since push_back is really slow I'd like to know the number of edges in advance
  int N = 0;
  for(auto it = halfedges.begin(); it->twin != nullptr; it++){
    // luckily, twin halfedges are stored one after the other so I just need
    // to skip the subsequent halfedge to not include the same information twice
    it++;
    N++;
  }

  // allocate the vectors with the exact size
  Rcpp::IntegerVector ind1(N), ind2(N), bp1(N), bp2(N);
  Rcpp::NumericVector x1(N), y1(N), x2(N), y2(N), mx1(N), my1(N), mx2(N), my2(N);

  auto it = halfedges.begin();
  for(size_t i = 0; i < N; i++){
      ind1[i] =  it->incidentFace->site->index + 1;
      ind2[i] =  it->twin->incidentFace->site->index + 1;

      x1[i] =  it->incidentFace->site->point.x;
      y1[i] =  it->incidentFace->site->point.y;
      x2[i] =  it->twin->incidentFace->site->point.x;
      y2[i] =  it->twin->incidentFace->site->point.y;

      mx1[i] =  it->destination->point.x;
      my1[i] =  it->destination->point.y;
      mx2[i] =  it->origin->point.x;
      my2[i] =  it->origin->point.y;

      // check if the current half edge is an infinite one
      bp1[i] =  isboundary(Vector2<real>(it->destination->point.x, it->destination->point.y), localbox);
      bp2[i] =  isboundary(Vector2<real>(it->origin->point.x, it->origin->point.y), localbox);

      // luckily, twin halfedges are stored one after the other so I just need
      // to skip the subsequent halfedge to not include the same information twice
      it++; it++;
    }

  Rcpp::CharacterVector names{"ind1", "ind2", "x1", "y1", "x2", "y2", "mx1", "my1", "mx2", "my2", "bp1", "bp2"};
  Rcpp::NumericMatrix mesh(ind1.size(), names.size());
  colnames(mesh) = names;
  rownames(mesh) = Rcpp::CharacterVector(ind1.size(), "");

  mesh(_, 0) = ind1;
  mesh(_, 1) = ind2;
  mesh(_, 2) = x1;
  mesh(_, 3) = y1;
  mesh(_, 4) = x2;
  mesh(_, 5) = y2;
  mesh(_, 6) = mx1;
  mesh(_, 7) = my1;
  mesh(_, 8) = mx2;
  mesh(_, 9) = my2;
  mesh(_,10) = bp1;
  mesh(_,11) = bp2;

  // 3. Construct the tri object
  // as retrieved from function "tri.mesh" of package tripack here's the structure
  // of the object tri
  // original tri object stores infromation about triangulation in a strange way
  // with pointers. i decided to go for a more intuitive way by creating a list

  /* the list neighbors is composed by n (number of sites) vector that contain for
   * each site the indeces of its neighbours in the Delanuay triangulation
   */
  Rcpp::List neighbors(x.size());
  for(size_t i=0; i<x.size(); i++)
    neighbors[i] = to_RcppInt(add_one<size_t>(triangulation.getNeighbors(i))); // retrieve information about neighbors

  Rcpp::List tri = Rcpp::List::create(Rcpp::Named("n") = Rcpp::IntegerVector(1,x.size()),
                                      Rcpp::Named("x") = Rcpp::NumericVector(x),
                                      Rcpp::Named("y") = Rcpp::NumericVector(y),
                                      Rcpp::Named("neighbours") = neighbors);
  tri.attr("class") = "tri.fake";

  // Construct the final del.vor object
  Rcpp::List res = Rcpp::List::create(Rcpp::Named("mesh") = mesh,
                                      Rcpp::Named("x") = coord,
                                      Rcpp::Named("tri.obj") = tri);
  res.attr("class") = "delvor";

  return res;
}
