# written by Federico Airoldi
#' Voronoi tesselation/Delanuay triangulation
#'
#' Computing Voronoi diagram and Delanuay tesselation for the specified set of points employing the C++
#' library \href{https://github.com/pvigier/FortuneAlgorithm}{MyGAL}.
#'
#' @param x x coordinates of the sites or a matrix with two columns containing both sites coordinates.
#' @param y y coordinates of the sites. Alternatively a single argument can be provided (see x).
#'
#' @return An invisible object of class "del.vor", a list, with the following components:
#' \describe{
#'    \item{mesh}{a matrix describing the Voronoi tesselation and the Delanuay triangulation.}
#'    \item{x}{a 2-column matrix containing the coordinates of the sites.}
#'    \item{tri.obj}{a "fake" tri object describing the triangulation similar to the one of the 
#'    package tri.mesh.}
#' }
#'
#' @details This function retrieves the Voronoi tesselation and the Delanuay triangulation of a given set
#' of points in the plane; results are returned in a list of three elements (see Value).
#' 
#' Each row of \bold{mesh} contains information about one of the edges of the tesselation and its dual:
#' \itemize{
#' \item \code{ind1} and \code{ind2}: indices of the sites to which the edge refers to; 
#' \item \code{x1} and \code{y1}: coordinates of the site denoted by ind1; 
#' \item \code{x2} and \code{y2}: coordinates of the site denoted by ind2; 
#' \item \code{mx1} and \code{my1}: coordinates of the first extreme, e1, of the Voronoi tesselation 
#' edge; 
#' \item \code{mx2} and \code{my2}: coordinates of the second extreme, e2, of the Voronoi tesselation 
#' edge; 
#' \item \code{bp1} and \code{bp2}: denote wether one of the direction of the Voronoi edge is infinite, either from the
#' side of e1 or e2. 
#' }
#' 
#' \bold{tri.obj} is a list of four elements that describe the Delanuay triangulation:
#' \itemize{
#' \item \code{n}: number of sites of the triangulation;
#' \item \code{x}: x coordinates of the sites;
#' \item \code{y}: y coordinates of the sites;
#' \item \code{neighbours}: a list of integer vectors where the i-th vectors contains the indeces of neighbours
#' sites of the i-th site in the triangulation.
#' }
#' 
#' @seealso \code{\link{plot.delvor}}
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' del.vor = delvor(x, y)
#' plot(del.vor)
#'
#' @export
delvor = function(x, y = NULL){
  X <- xy.coords(x, y)
  x <- cbind(X$x, X$y)

  if (dim(x)[1] <= 2)
    stop("At least three non-collinear points are required")
  
  invisible(.computeVoronoiRcpp(X$x,X$y))
}
