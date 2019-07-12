# written by Federico Airoldi
my.delvor = function(x, y = NULL){
  X <- xy.coords(x, y)
  x <- cbind(X$x, X$y)
  if (dim(x)[1] <= 2) {
    stop("At least three non-collinear points are required")
  }
  invisible(computeVoronoiRcpp(X$x,X$y))
}
