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
#' a.hull = my.ahull(x, y)
#' plot(a.hull)
#'
#' @export
my.ahull = function (x, y = NULL, alpha){
  ashape.obj <- my.ashape(x, y, alpha)
}
