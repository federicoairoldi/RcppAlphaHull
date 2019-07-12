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
}*/
 
/*
template <typename T>
bool isboundary(const Vector2<T>& point, const Box<T>& box){
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

std::vector<std::size_t> plusone(const std::vector<std::size_t>& v){ 
  std::vector<std::size_t> newvett = v;
  for(int i=0; i<newvett.size(); i++)
    newvett[i]+=1;
  return newvett; 
}

// [[Rcpp::export]]
Rcpp::List computeVoronoiRcpp(const Rcpp::NumericVector x, const Rcpp::NumericVector y) {
  // useless if performed by xy.coord in R
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
  mygal::Diagram<ftype>::Site *site1, *site2;
  auto halfedges = diagram.getHalfEdges();
  
  // since push_back is really slow I'd like to know the number of edges in advance
  int N = 0;
  for(auto it = halfedges.begin(); it->twin != nullptr; it++){
    it++;
    N++;
  }
  
  // allocated the vector with the exact size
  Rcpp::IntegerVector ind1(N), ind2(N), bp1(N), bp2(N);
  Rcpp::NumericVector x1(N), y1(N), x2(N), y2(N), mx1(N), my1(N), mx2(N), my2(N);
  
  auto it = halfedges.begin();
  for(int i = 0; i < N; i++){
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
      
      // checking if the half edge is an infinite one
      bp1[i] =  isboundary(Vector2<ftype>(it->destination->point.x, it->destination->point.y), localbox);
      bp2[i] =  isboundary(Vector2<ftype>(it->origin->point.x, it->origin->point.y), localbox);
      
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
  
  // stuff for tri object (maybe this has better to be done in R)
  // as retrieved from function "tri.mesh" of package tripack here's the structure
  // of the object tri
  Rcpp::List neighbors(x.size());
  for(int i=0; i<x.size(); i++)
    neighbors[i] = plusone(triangulation.getNeighbors(i));
  
  Rcpp::List tri = Rcpp::List::create(Rcpp::Named("n") = x.size(),
                                      Rcpp::Named("x") = Rcpp::NumericVector(x),
                                      Rcpp::Named("y") = Rcpp::NumericVector(y),
                                      Rcpp::Named("neighbours") = neighbors,
                                      Rcpp::Named("lc") = 0,
                                      Rcpp::Named("call") = 1); 
  tri.attr("class") = "tri.fake";
  
  Rcpp::List res = Rcpp::List::create(Rcpp::Named("mesh") = mesh,
                                      Rcpp::Named("x") = coord,
                                      Rcpp::Named("tri.obj") = tri);
  res.attr("class") = "delvor";
  
  return res;
}

/*** R
# loading required packages
require(alphahull)
require(rbenchmark)

# sampling some random points
n = 8
errtest = 0; # dummy for testing exiting error (maybe this would be handled by R)

set.seed(123)
x = runif(n)
y = runif(n+errtest)

system.time(computeVoronoiRcpp(x, y))
system.time(delvor(x, y))

vorcpp = computeVoronoiRcpp(x,y)
vorR = delvor(x, y)
# benchmark(computeVoronoiRcpp(x, y), delvor(x, y))
*/
