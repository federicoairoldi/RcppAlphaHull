#' Alpha-shape computation
#'
#' Computes the alpha-shape for the given set of points
#'
#' @param {x,y} coordinates of the sites. ALternatively a single argument x can be provided (see Details).
#' @param alpha value of alpha
#' 
#' @return A list with the following components: 
#' \describe{
#'    \item{edges}{a mesh describing the alpha shape, this object is a submatrix of the mesh matrix 
#'    contained in a "delvor" object since the alpha shape is a subset of the Delanuay triangulation.}
#'    \item{length}{length of the alpha shape.}
#'    \item{alpha}{the value of alpha for which the shape is computed.}
#'    \item{alpha.extremes}{containes the indices of those sites that are alpha extremes.}
#'    \item{delvor.obj}{a delvor object returned by the function delvor, if a delvor object is provided
#'    instead of the sites coordinates, this field contains such object}
#' }
#'
#' @details 
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' a.shape = ashape(x, y, alpha = 0.8)
#' plot(a.shape)
#'
#' @export
ashape = function(x, y = NULL, alpha){
  if (alpha < 0)
    stop("Parameter alpha must be greater or equal to zero")

  if (!inherits(x, "delvor"))
    dd.obj <- delvor(x, y)
  else
    dd.obj <- x

  invisible(.computeAshapeRcpp(dd.obj, alpha))
}
