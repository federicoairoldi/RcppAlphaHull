#' Voronoi tesselation/Delanuay triangulation plot
#'
#' Plot of an object "del.vor" with a "tri.mod" tri object.
#'
#' @param x object of class del.vor.
#' @param add if TRUE the plot is added to the active graphic window.
#' @param wlines a string specifying what has to be plotted:
#' \itemize{
#'   \item "vor": shows the Voronoi tesselation;
#'   \item "del": shows the Delanuay triangulation plot;
#'   \item "both": shows both of the above structures.
#' }
#' @param wpoints if true, the sites are added to the plot.
#' @param number if true, the plot shows the indeces of the sites between their locations.
#' @param col specifies in a vector the colors to be used for the different objects to be plotted and the
#' order is:
#' \itemize{
#'   \item col[1] -> color of the points;
#'   \item col[2] -> color of Delanuay triangulation;
#'   \item col[3] -> color of Voronoi tesselation
#'   \item col[4] -> color for the numbers
#' } 
#' Alternatively just one color can be provided.
#' @param xlim x axis limits.
#' @param ylim y axis limits.
#' @param ... graphical arguments to be passes to methods (see \code{\link[graphics]{par}})
#'
#' @seealso \code{\link{delvor}}
#'
#' @return
#'
#' @examples
#' x = runif(10)
#' y = runif(10)
#' del.vor = delvor(x, y)
#'
#' plot.delvor(vor.del, wlines = "both", wpoints = FALSE, number = TRUE, 
#'             col = c("black", "blue", "red", "black"))
#'
#' @export
plot.delvor = function (delvor.obj, add = FALSE, wlines = c("both", "del", "vor"), wpoints = TRUE,
                        number = FALSE, col = NULL, xlim = NULL, ylim = NULL, ...){
    wlines <- match.arg(wlines)
    if (is.null(class(delvor.obj)) || class(delvor.obj) != "delvor") {
      cat("Argument is not of class delvor!!\n")
      return(invisible())
    }
    if (is.null(col))
      col <- c(1, 1, 1, 1)
    else
      col <- rep(col, length.out = 4)

    plot.del <- switch(wlines, both = TRUE, del = TRUE, vor = FALSE)
    plot.vor <- switch(wlines, both = TRUE, del = FALSE, vor = TRUE)

    if (!add) {
      if (is.null(xlim))
        xlim <- range(delvor.obj$x[, 1])
      if (is.null(ylim))
        ylim <- range(delvor.obj$x[, 2])
      plot.default(0, 0, type = "n", xlim = xlim, ylim = ylim, axes = FALSE, ...)
      axis(side = 1)
      axis(side = 2)
    }

    if (number) {
      xoff <- 0.02 * diff(range(delvor.obj$x[, 1]))
      yoff <- 0.02 * diff(range(delvor.obj$x[, 2]))
      text(delvor.obj$x[, 1] + xoff, delvor.obj$x[, 2] + yoff, 1:(dim(delvor.obj$x)[1]),
           col = col[4], ...)
    }

    if (plot.vor) {
      n.edge <- dim(delvor.obj$mesh)[1]
      for (i in 1:n.edge) {
        lty.vor = ifelse(delvor.obj$mesh[i, "bp1"] == 1 | delvor.obj$mesh[i, "bp2"] == 1, 2, 1)
        segments(delvor.obj$mesh[i, "mx1"], delvor.obj$mesh[i, "my1"], delvor.obj$mesh[i, "mx2"],
                 delvor.obj$mesh[i, "my2"], lty = lty.vor, col = col[3], ...)
      }
    }

    if (plot.del) {
      neigh = delvor.obj$tri.obj$neighbours
      for(i in 1:(length(neigh)))
        for(j in neigh[[i]][ which(neigh[[i]]>i) ])
          lines( c(delvor.obj$x[i,1], delvor.obj$x[j,1]),
                 c(delvor.obj$x[i,2], delvor.obj$x[j,2]) , col = col[2], lty = 1, ... )
    }
    
    if (wpoints)
      points(delvor.obj$x, col = col[1], ...)
}

