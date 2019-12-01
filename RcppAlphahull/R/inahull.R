#' Determines if the provided points fall inside the \eqn{\alpha}-hull or not.
#'
#' Determines if the provided points fall inside the \eqn{\alpha}-hull or not by calling an external C++
#' function.
#'
#' @param ahull.onj an object class "ahull" like the one returned by the function \code{\link{ahull}}
#' @param x x coordinates of the points to be checked. Alternatively, a matrix with two columns containing both 
#' coordinates.
#' @param y y coordinates of the points to be checked.
#'
#' @return A logical vector of the same length of the number of points and such that the i-th element denotes 
#' if the i-th point is in the \eqn{\alpha}-hull or not.
#'
#' @seealso \code{\link{ahull}}, \code{\link{complement}}
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' inahull(ahull.obj, x, y)
#'
#' @export
inahull = function (ahull.obj, x, y = NULL, alpha){
  X <- xy.coords(x, y)
  
  if (!inherits(ahull.obj, "ahull"))
    stop("The provided object is not an ahull one")
  
  return(.inahullRcpp(ahull.obj$complement, X$x, X$y))
}
