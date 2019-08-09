#' @title RcppAlphahull
#'
#' @details 
#' RcppAlphahull is package aimed to compute Voronoi diagrams and Delanuay tesselations for sets of 
#' points using the C++ package \href{https://github.com/pvigier/FortuneAlgorithm}{MyGAL}. The package is
#' inspired by the R/Fortran package alphahull which does the same job, but much slower. Plot of some 
#' graphs are done by using this package functions. 
#'
#' @description Voronoi tesselation/Delanuay triangulation, \eqn{\alpha}-shape and \eqn{\alpha}-hull 
#' computation using Rcpp.
#'
#' @author Federico Airoldi \email{federico2.airoldi@@mail.polimi.it}
#' 
#' @references
#' Edelsbrunner, H., Kirkpatrick, D. G., Seidel, R. 1983, 'On the Shape of a Set of 
#' Points in the Plane', \emph{IEEE Transactions on Information Theory}, vol. 29, no. 4, pp. 551-559.
#'
#' @useDynLib RcppAlphahull
#' @import alphahull
#' @importFrom Rcpp evalCpp
#' @exportPattern "[[:alpha:]]+"
#' 
#' @name RcppAlphahull
#' @docType package
#' @keywords internal
"_PACKAGE"

# The following block is used by usethis to automatically manage
# roxygen namespace tags. Modify with care!
## usethis namespace: start
## usethis namespace: end
NULL
