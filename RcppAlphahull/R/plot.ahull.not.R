#' \eqn{\alpha}-hull plot
#'
#' Plot of an object "ahull".
#'
#' @param ahull.obj object of class ahull
#' @param add if TRUE the plot is added to the active graphic window.
#' @param do.shape if TRUE the \eqn{\alpha}-shape for the same sites and same level alpha is plotted.
#' @param wlines a string specifying what has to be plotted:
#' \itemize{
#'   \item "none": nothing is added to the plot;
#'   \item "vor": shows the Voronoi tesselation;
#'   \item "del": shows the Delanuay triangulation plot;
#'   \item "both": shows both of the above structures.
#' }
#' @param wpoints if true, the sites are added to the plot.
#' @param number if true, the plot shows the indeces of the sites between their locations.
#' @param col specifies in a vector the colors to be used for the different objects to be plotted and the
#' order is:
#' \itemize{
#'   \item col[1] -> color of the \eqn{\alpha}-hull;
#'   \item col[2] -> color of the \eqn{\alpha}-shape;
#'   \item col[3] -> color of the points;
#'   \item col[4] -> color of Delanuay triangulation;
#'   \item col[5] -> color of Voronoi tesselation
#'   \item col[6] -> color for the numbers
#' } 
#' Alternatively just one color can be provided.
#' @param xlim x axis limits.
#' @param ylim y axis limits.
#' @param ... graphical arguments to be passes to methods (see \code{\link[graphics]{par}})
#'
#' @seealso \code{\link{ahull}}
#'
#' @return
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' ahull.obj = ahull(x, y, alpha = 0.5)
#'
#' plot.ahull(ahull.obj, wlines = "none", wpoints = TRUE)
#'
#' @export
plot.ahull.not = function (ahull.obj, add = FALSE, do.shape = FALSE, wlines = c("none", "both", "del", "vor"), 
                       wpoints = TRUE, number = FALSE, col = NULL, xlim = NULL, ylim = NULL, ...){
  wlines <- match.arg(wlines)
  if (is.null(class(ahull.obj)) || class(ahull.obj) != "ahull") {
    cat("Argument is not of class ahull!!\n")
    return(invisible())
  }
  if (is.null(col))
    col <- c(1, 1, 1, 1, 1, 1)
  else
    col <- rep(col, length.out = 6)
  
  plot.del <- switch(wlines, both = TRUE, del = TRUE, vor = FALSE)
  plot.vor <- switch(wlines, both = TRUE, del = FALSE, vor = TRUE)
  
  l = 1000
  xmin = min(ahull.obj$ashape.obj$x[,1])
  xmax = max(ahull.obj$ashape.obj$x[,1])
  ymin = min(ahull.obj$ashape.obj$x[,2])
  ymax = max(ahull.obj$ashape.obj$x[,2])
  xseq = seq(xmin,xmax,length=l)
  yseq = seq(ymin,ymax,length=l)
  xy = expand.grid(x = xseq, y = yseq)
  res = inahull(ahull.obj, xy[,1], xy[,2])
  contour(xseq, yseq, matrix(res,l), levels = TRUE, drawlabels = FALSE, add = add, col = col[1])
}

