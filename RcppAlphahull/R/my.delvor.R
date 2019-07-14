# written by Federico Airoldi
#' Voronoi diagram/Delanuay tesselation
#'
#' Computing Voronoi diagram and Delanuay tesselation for the specified set of points
#'
#' @param x coordinates of the sites
#' @param y coordinates of the sites
#'
#' @return An invisible object of class "del.vor", a list, with the following components:
#' \describe{
#'    \item{mesh}{a mesh describing the diagram}
#'    \item{x}{nx2 matrix containing the coordinates of the sites}
#'    \item{tri.obj}{a "fake" tri object describing the triangulation}
#' }
#'
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' vor.del = my.delvor(x, y)
#'
#' @export
my.delvor = function(x, y = NULL){
  X <- xy.coords(x, y)
  x <- cbind(X$x, X$y)
  if (dim(x)[1] <= 2)
    stop("At least three non-collinear points are required")

  invisible(computeVoronoiRcpp(X$x,X$y))
}
