#' Complementary \eqn{\alpha}-hull computation
#'
#' Computes the complement of an \eqn{\alpha}-hull for the given value of \eqn{\alpha} provided.
#'
#' @param x coordinates of the sites. Alternatively, a matrix with two columns containing both sites 
#' coordinates or an object of class delvor.
#' @param y y coordinates of the sites; do not insert if x is not a vector of coordinates (see x).
#' @param alpha a strictly positive value for \eqn{\alpha}.
#'
#' @return A matrix containing information about balls and halfplanes constituting the complement of the 
#' \eqn{\alpha} hull, each row describes either an open ball or an open halfplane such that balls are 
#' saved in the following way:
#' \itemize{
#'   \item \code{c1}: x coordinate of the center of the ball;
#'   \item \code{c2}: y coordinate of the center of the ball;
#'   \item \code{r}: radius of the ball;
#' }
#' If the row refers to an halfplane then \code{complement[i,1:3]} has the following forms:
#' \itemize{
#'   \item x>a+bx: \code{(a,b,-1)};
#'   \item x<a+bx: \code{(a,b,-2)};
#'   \item x>a:    \code{(a,0,-3)};
#'   \item x<a:    \code{(a,0,-4)}.
#' }
#'
#' @seealso \code{\link{ahull}}
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' alpha = 2
#' ahull.compl = complement(x, y, alpha)
#'
#' @export
complement = function (x, y = NULL, alpha){
  if (!inherits(x, "delvor"))
    delvor.obj <- delvor(x, y)
  else
    delvor.obj <- x
  
  invisible(.computeComplement(delvor.obj$mesh, alpha))
}
