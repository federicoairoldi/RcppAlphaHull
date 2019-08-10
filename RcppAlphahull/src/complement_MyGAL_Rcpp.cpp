#ifndef _UTILITIES_ALPHAHULL_
#define _UTILITIES_ALPHAHULL_

#include <Rcpp.h>
#include <cmath>
#include <iostream>
#include "newClasses/Rect.h"
#include "newClasses/Ball.h"
#include "newClasses/Segment.h"
#include "newClasses/HalfPlane.h"
#include "MyGAL/Vector2.h"
using namespace Rcpp;

// [[Rcpp::export(".computeComplement")]]
Rcpp::NumericMatrix computeComplement(const Rcpp::NumericMatrix& mesh, const long double& alpha){
  typedef long double real;
  
  std::vector<Ball<real>> balls; // vector that contains the open balls that form the complement
  std::vector<HalfPlane<real>> halfplanes; // vector that contains the open halfplanes that form the complement
  std::vector<size_t> rows_balls; // vectors that contains the rows to which balls refer to
  std::vector<size_t> rows_halfplanes; // vectors that contains the rows to which halfplanes refer to

  // reserving memory for the worst case
  balls.reserve(4*mesh.rows());
  halfplanes.reserve(mesh.rows());
  rows_balls.reserve(5*mesh.rows());
  rows_halfplanes.reserve(5*mesh.rows());

  for(int i=0; i<mesh.rows(); i++){
    Rcpp::checkUserInterrupt();

    bool bp1 = (mesh(i,10) == 1), bp2 = (mesh(i,11) == 1);
    // computing rects
    Vector2<real>  p(mesh(i,2), mesh(i,3)); // 1st site
    Vector2<real>  q(mesh(i,4), mesh(i,5)); // 2nd site
    Vector2<real> e1(mesh(i,6), mesh(i,7)); // 1st extreme of the voronoi segment
    Vector2<real> e2(mesh(i,8), mesh(i,9)); // 2nd extreme of the voronoi segment
    Rect<real> r(p,q), bis(e1,e2); // rect through p and q and bisectrix of p and q
    Segment<real> vor_edge(e1,e2); // NB: infinite edge are clipped!! I will handle this later (*)
    // eventual halfplanes to add or evaluate
    HalfPlane<real> h1(r, r.eval(e1)==1? true: false ), // halfplane for bp1 = 1
                    h2(r, r.eval(e2)==1? true: false ); // halfplane for bp2 = 1

    // computing distances from p to endpoints of the voronoi edge (for infinite segments I use infinity)
    real d1 = bp1? std::numeric_limits<real>::infinity(): p.getDistance(e1),
         d2 = bp2? std::numeric_limits<real>::infinity(): p.getDistance(e2);

    // searching extremes with distance alpha on the rect "bis"
    std::vector<Vector2<real>> points = bis.getDistNeigh(p,alpha);
    
    //std::cout << "Edge: " << mesh(i,0) << " " << mesh(i,1) << std::endl;

    // add ball or halfplane for side of e1
    if(bp1){ // if bp1 = 1 then I need to add an an halfplane
      halfplanes.push_back(HalfPlane<real>(r, r.eval(e1)==1? true: false ));
      rows_halfplanes.push_back(i);
      //std::cout << "Adding upper halfplane" << std::endl;
    }
    else if(d1>=alpha){ // add a ball but only if e1 is at least distant alpha from p
      balls.push_back(Ball<real>(e1,d1));
      rows_balls.push_back(i);
      //std::cout << "Adding ball: " << e1 << " " << d1 << std::endl;
    }

    // add ball or halfplane for side of e2
    if(!bp1 && bp2){ // if bp2 = 1 then I need to add an halfplane
      halfplanes.push_back(HalfPlane<real>(r, r.eval(e2)==1? true: false ));
      rows_halfplanes.push_back(i);
      //std::cout << "Adding lower halfplane" << std::endl;
    }
    else if(d2>=alpha){ // add a ball but only if e2 is at least distant alpha from p
      balls.push_back(Ball<real>(e2,d2));
      rows_balls.push_back(i);
      //std::cout << "Adding ball: " << e2 << " " << d2 << std::endl;
    }

    bool same_side1 = (h1.isIn(e1) == h1.isIn(e2)), // e1 and e2 are both in the upper (right) halfplane
         same_side2 = (h2.isIn(e1) == h2.isIn(e2)); // e1 and e2 are both in the lower (left) halfplane
    // add eventual ball for intersection points
    for(size_t k=0; k<points.size(); k++){
      bool add = false;
      add = inside(vor_edge,points[k]); // if point[k] falls inside the Voronoi edge then for sure I need to add a ball
      // std::cout << "points[" << k << "] inside?" << inside(vor_edge,points[k]) << std::endl;
      if( bis.eval(points[k])!=0 ){
        // std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH! " << "points[" << k << "] in rect?" << bis.eval(points[k]) << std::endl;
      }
      // (*) here I handle that problem
      // if point[k] is not in the finite version of the Voronoi edge then I need to check whether orù
      // not it belongs to an infinite edge
      add = add || (bp1 && h1.isIn(points[k]) && (same_side1? alpha>=d2: true)); // checking if belongs to h1 (if bp1 = 1) AND distant at least alpha if the other edge estreme is in the same side
      add = add || (bp2 && h2.isIn(points[k]) && (same_side2? alpha>=d1: true)); // checking if belongs to h2 (if bp2 = 1) AND distant at least alpha if the other edge estreme is in the same side
      if(add){
        balls.push_back(Ball<real>(points[k], p.getDistance(points[k])));
        rows_balls.push_back(i);
        // std::cout << "Adding ball: " << points[k] << " " << p.getDistance(points[k]) << std::endl;
      }
    }
    // std::cout << std::endl;
  }

  // constructing the output matrix
  Rcpp::NumericMatrix complement(balls.size()+halfplanes.size(), 19);
  for(size_t i=0; i<balls.size(); i++){
    size_t idx = rows_balls[i];
    complement(i,0) = balls[i].center().x; // c1
    complement(i,1) = balls[i].center().y; // c2
    complement(i,2) = balls[i].radius();   // r
  
    for(int j=0; j<mesh.cols(); j++) // mesh informations
      complement(i,3+j) = mesh(idx,j);
    
    // retrieving the arc information
    Vector2<real> p(mesh(idx,2), mesh(idx,3)), q(mesh(idx,4), mesh(idx,5));
    Vector2<real> pc = p - balls[i].center(), qc = q - balls[i].center();
    Vector2<real> v = (pc+qc);
    v = 1/v.getNorm()*v;
    real theta = std::acos((v.x*pc.x+v.y*pc.y)/(v.getNorm()*pc.getNorm()));
    
    complement(i,16) = v.x;
    complement(i,17) = v.y;
    complement(i,18) = theta;
  }
  for(size_t i=0; i<halfplanes.size(); i++){
    size_t idx = rows_halfplanes[i];
    complement(balls.size()+i,0) = halfplanes[i].rectIntercept();
    complement(balls.size()+i,1) = halfplanes[i].rectSlope();
    complement(balls.size()+i,2) = (!halfplanes[i].isVertical()? -1: -3) - (halfplanes[i].getSide()==-1);
    
    for(int j=0; j<mesh.cols(); j++)
      complement(balls.size()+i,3+j) = mesh(idx,j);
  }
  colnames(complement) = Rcpp::CharacterVector::create("c1", "c2", "r", "ind1", "ind2", "x1", "y1", 
                                                       "x2", "y2", "mx1", "my1", "mx2", "my2", "bp1", 
                                                       "bp2", "ind", "v.x", "v.y", "theta");
  
  return complement;
};

#endif