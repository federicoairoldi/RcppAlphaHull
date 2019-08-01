#' Alpha-hull computation
#'
#' Computes the complement of an alpha hull 
#'
#' @param x coordinates of the sites
#' @param y coordinates of the sites
#' @param alpha positive parameter  
#'
#' @return A matrix containing information about balls and halfplanes constituting the complement of the 
#' alpha hull
#'
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' alpha = 2
#' ahull.compl = complement(x, y, alpha)
#'
#' @export
complement = function (x, y = NULL, alpha){
  if (!inherits(x, "ashape"))
    ashape.obj <- ashape(x, y, alpha)
  else
    ashape.obj <- x
  
  invisible(.computeComplement(ashape.obj))
}
