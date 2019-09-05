#' \eqn{\alpha}-hull computation
#'
#' Computes the \eqn{\alpha}-hull for the given set of points.
#'
#' @param x x coordinates of the sites. Alternatively, a matrix with two columns containing both sites 
#' coordinates, an object of class delvor or an object of class ashape.
#' @param y y coordinates of the sites; do not insert if x is not a vector of coordinates (see x).
#' @param alpha a strictly positive value for \eqn{\alpha} (NULL if x is of class ashape).
#'
#' @return A list with the following components:
#' \itemize{
#' \item \code{arcs}: a matrix containing the arcs that form the boundary of the \eqn{\alpha}-hull;
#' \item \code{length}: length of the \eqn{\alpha}-hull boundary;
#' \item \code{complement}: a matrix describing the complement of the \eqn{\alpha}-hull, see function
#' \code{\link{complement}} for a detailed description;
#' \item \code{alpha}: the value of \eqn{\alpha} for which the shape is computed;
#' \item \code{ashape}: output of function \code{\link{ashape}}.
#' }
#'
#' @seealso \code{\link{delvor}}, \code{\link{ashape}}, \code{\link{complement}}
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' a.hull = ahull(x, y, alpha = 0.8)
#' plot(a.hull)
#'
#' @export
ahull = function (x, y = NULL, alpha){
  if (!inherits(x, "ashape"))
    ashape.obj <- ashape(x, y, alpha)
  else
    ashape.obj <- x

  invisible(.computeAhullRcpp(ashape.obj))
}
