#include <Rcpp.h>
#include <iostream>
#include <MyGAL/FortuneAlgorithm.h>

using namespace mygal;
using namespace Rcpp;

// returns if the point "point" is at the boundary of the box "box"
template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
  if( ( point.x == box.left | point.x == box.right  ) 
        & point.y <= box.top & point.y >= box.bottom )
    return true;
  if( ( point.y == box.bottom | point.y == box.top  ) 
        & point.x <= box.right & point.x >= box.left )
    return true;
  return false;
}

// [[Rcpp::export]]
Rcpp::List computeVoronoiRcpp(const Rcpp::NumericVector& x, const Rcpp::NumericVector& y) {
  if( x.size() != y.size() ){
    Rcpp::Rcerr << "Error! x and y vectors don't have same length!" << std::endl;
    // i'd like also to exit... maybe not by returning an invalid object
    return Rcpp::List::create();
  }
  
  typedef long double ftype;  
  
  std::vector<Vector2<ftype>> points;
  
  for(int i=0; i<x.size(); i++)
    points.push_back(Vector2<ftype>(x[i], y[i]));
  
  // procedure suggested by the author of the library MyGAL
  auto algorithm = FortuneAlgorithm<ftype>(points); // Initialize an instance of Fortune's algorithm
  algorithm.construct(); // Construct the diagram
  
  Box<ftype> localbox{0, 0, 1, 1};
  algorithm.bound(&localbox); // Bound the diagram
  auto diagram = algorithm.getDiagram(); // Get the constructed diagram
  //diagram.intersect(Box<ftype>{0, 0, 2, 2}); // Compute the intersection between the diagram and a box
  
  auto triangulation = diagram.computeTriangulation();
  
  // extracting information and generating the R object
  // matrix of coordinates
  Rcpp::NumericMatrix coord(x.size(), 2);
  coord(_, 0) = x;
  coord(_, 1) = y;
  
  // data.frame mesh (contains delanuay/voronoi information)
  Rcpp::IntegerVector ind1, ind2, bp1, bp2;
  Rcpp::NumericVector x1, y1, x2, y2, mx1, my1, mx2, my2;
  
  auto halfedges = diagram.getHalfEdges();
  for(auto it = halfedges.begin(); it != halfedges.end(); it++)
    if(it->twin != nullptr){
      ind1.push_back( it->incidentFace->site->index + 1 );
      ind2.push_back( it->twin->incidentFace->site->index + 1 );
      
      x1.push_back( it->incidentFace->site->point.x );
      y1.push_back( it->incidentFace->site->point.y );
      x2.push_back( it->twin->incidentFace->site->point.x );
      y2.push_back( it->twin->incidentFace->site->point.y );
      
      mx1.push_back( it->destination->point.x );
      my1.push_back( it->destination->point.y );
      mx2.push_back( it->origin->point.x );
      my2.push_back( it->origin->point.y );
      
      // checking if the half edge is an infinite one
      bp1.push_back(isboundary(Vector2<ftype>(it->destination->point.x, it->destination->point.y), localbox));
      bp2.push_back(isboundary(Vector2<ftype>(it->origin->point.x, it->origin->point.y), localbox));
      
      // luckily, twin halfedges are stored one after the other so I just need 
      // to skip the subsequent halfedge to not include the same information twice
      it++;
    }
  
  Rcpp::DataFrame mesh = 
    Rcpp::DataFrame::create(Rcpp::Named("ind1") = ind1,
                            Rcpp::Named("ind2") = ind2,
                            Rcpp::Named("x1") = x1,
                            Rcpp::Named("y1") = y1,
                            Rcpp::Named("x2") = x2,
                            Rcpp::Named("y2") = y2,
                            Rcpp::Named("mx1") = mx1,
                            Rcpp::Named("my1") = my1,
                            Rcpp::Named("mx2") = mx2,
                            Rcpp::Named("my2") = my2,
                            Rcpp::Named("bp1") = bp1,
                            Rcpp::Named("bp2") = bp2);
  
  // stuff for tri object (maybe this has better to be done in R)
  // add here the stuff
  
  Rcpp::List res = Rcpp::List::create(Rcpp::Named("mesh") = mesh,
                                      Rcpp::Named("x") = coord,
                                      Rcpp::Named("temp") 
                                        = Rcpp::NumericVector::create(localbox.left, localbox.right, localbox.bottom, localbox.top));
    
  return res;
}

/*** R
# sampling some random points
n = 50
errtest = 0; # dummy for testing exiting error (maybe this would be handled by R)
set.seed(52)
x = runif(n)
y = runif(n+errtest)

# loading required packages
require(alphahull)
require(rbenchmark)

print("executing test")
vorcpp = computeVoronoiRcpp(x, y)
vorR = delvor(x, y)
# benchmark(computeVoronoiRcpp(x, y), delvor(x, y))
*/
