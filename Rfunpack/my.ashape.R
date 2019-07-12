my.ashape = function(x, y = NULL, alpha){
  if (alpha < 0) {
    stop("Parameter alpha must be greater or equal to zero")
  }
  if (!inherits(x, "delvor")) {
    dd.obj <- my.delvor(x, y)
  }
  else {
    dd.obj <- x
  }
  invisible(ashape(dd.obj, alpha = alpha))
}
