#include <Rcpp.h>
#include <iostream>
#include <MyGAL/FortuneAlgorithm.h>

using namespace mygal;
using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::List computeVoronoiRcpp(const Rcpp::NumericVector& x, const Rcpp::NumericVector& y) {
  // if( x.size() != y.size() ) std::err ???
    
  std::vector<Vector2<double>> points;
  
  for(int i=0; i<x.size(); i++)
    points.push_back(Vector2<double>(x[i], y[i]));
  
  //auto points = std::vector<Vector2<double>>{{0.354, 0.678}, {0.632, 0.189}, {0.842, 0.942}}; // Generate some points
  auto algorithm = FortuneAlgorithm<double>(points); // Initialize an instance of Fortune's algorithm
  algorithm.construct(); // Construct the diagram
  
  algorithm.bound(Box<double>{-0.05, -0.05, 1.05, 1.05}); // Bound the diagram
  auto diagram = algorithm.getDiagram(); // Get the constructed diagram
  diagram.intersect(Box<double>{0.0, 0.0, 1.0, 1.0}); // Compute the intersection between the diagram and a box
  
  auto triangulation = diagram.computeTriangulation();
  
  Rcpp::NumericMatrix coord(x.size(), 2);
  for(int i=0; i<x.size(); i++){
    coord(i,0) = x[i];
    coord(i,1) = y[i];
  }
  
  Rcpp::DataFrame mesh = 
    Rcpp::DataFrame::create(Rcpp::Named("ind1") = 1,
                            Rcpp::Named("ind2") = 1,
                            Rcpp::Named("x1") = 1,
                            Rcpp::Named("y1") = 1,
                            Rcpp::Named("x2") = 1,
                            Rcpp::Named("y2") = 1,
                            Rcpp::Named("mx1") = 1,
                            Rcpp::Named("my1") = 1,
                            Rcpp::Named("mx2") = 1,
                            Rcpp::Named("my2") = 1,
                            Rcpp::Named("bp1") = 1,
                            Rcpp::Named("bp2") = 1);
  
  Rcpp::List res = Rcpp::List::create(Rcpp::Named("mesh") = mesh,
                                      Rcpp::Named("x") = coord,
                                      Rcpp::Named(""));
    
  return res;
}

/*** R
n = 5
set.seed(100)
x = runif(n)
y = runif(n)
computeVoronoiRcpp(x, y)
*/
