#include <Rcpp.h>
#include <iostream>
#include <MyGAL/FortuneAlgorithm.h>

using namespace mygal;
using namespace Rcpp;

/*
// returns if the point "point" is at the boundary of the box "box"
template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
  if( ( (point.x == box.left) | (point.x == box.right)  ) 
        & point.y <= box.top & point.y >= box.bottom )
    return true;
  if( ( point.y == box.bottom | point.y == box.top  ) 
        & point.x <= box.right & point.x >= box.left )
    return true;
  return false;
}

template <typename T>
bool isboundary2(const Vector2<T>& point, const Box<T>& box){
  T eps(10e-5);
  if( ( ( point.x >= (box.left - eps) & point.x <= (box.left + eps) )
          | ( point.x >= (box.right - eps) & point.x <= (box.right + eps) )  ) 
        & point.y <= box.top & point.y >= box.bottom )
        return true;
  if( ( ( point.y >= (box.top - eps) & point.y <= (box.top + eps) )
          | ( point.y >= (box.bottom - eps) & point.y <= (box.bottom + eps) )  ) 
        & point.x <= box.right & point.x >= box.left )
        return true;
  return false;
}*/

template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
  T eps(10e-5);
  if( ( std::abs((point.x-box.left)/box.left) < eps | std::abs((point.x-box.right)/box.right) < eps  ) 
        & point.y <= box.top & point.y >= box.bottom )
        return true;
  if( ( std::abs((point.y-box.top)/box.top) < eps | std::abs((point.y-box.bottom)/box.bottom) < eps  ) 
        & point.x <= box.right & point.x >= box.left )
        return true;
  return false;
}

// [[Rcpp::export]]
Rcpp::List computeVoronoiRcpp(const Rcpp::NumericVector& x, const Rcpp::NumericVector& y) {
  if( x.size() != y.size() )
    Rcpp::stop("Error! x and y vectors don't have same length!");
  
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
  
  // matrix mesh (contains delanuay/voronoi information)
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
  
  // stuff for tri object (maybe this has better to be done in R)
  // as retrieved from function "tri.mesh" of package tripack here's the structure
  // of the object tri
  
  Rcpp::List tri = Rcpp::List::create(Rcpp::Named("n") = x.size(),
                                      Rcpp::Named("x") = x,
                                      Rcpp::Named("y") = y,
                                      Rcpp::Named("tlist") = 1,
                                      Rcpp::Named("tlptr") = 1,
                                      Rcpp::Named("tlend") = 1,
                                      Rcpp::Named("tlnew") = 1,
                                      Rcpp::Named("nc") = 0,
                                      Rcpp::Named("lc") = 0,
                                      Rcpp::Named("call") = 1); 
  tri.attr("class") = "tri";
  
  Rcpp::List res = Rcpp::List::create(Rcpp::Named("mesh") = mesh,
                                      Rcpp::Named("x") = coord,
                                      Rcpp::Named("tri.obj") = tri,
                                      Rcpp::Named("temp") = Rcpp::NumericVector::create(localbox.left, localbox.right, localbox.bottom, localbox.top));
  res.attr("class") = "delvor";
  
  return res;
}

/*** R
# loading required packages
require(alphahull)
require(rbenchmark)

# sampling some random points
n = 50
errtest = 0; # dummy for testing exiting error (maybe this would be handled by R)

set.seed(309)
x = runif(n)
y = runif(n+errtest)
  
vorcpp = computeVoronoiRcpp(x, y)
vorR = delvor(x,  y)

# benchmark(computeVoronoiRcpp(x, y), delvor(x, y))
*/
