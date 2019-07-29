#' Alpha-hull computation
#'
#' Computes the alpha-hull for the given set of points
#'
#' @param x coordinates of the sites
#' @param y coordinates of the sites
#'
#' @return A list with the following components
#'
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' a.hull = ahull(x, y)
#' plot(a.hull)
#'
#' @export
ahull = function (x, y = NULL, alpha){
  print("using RcppAlphahull ahull")

  ashape.obj <- ashape(x, y, alpha)
  invisivìble(computeAhullRcpp(ashape.obj))
}
