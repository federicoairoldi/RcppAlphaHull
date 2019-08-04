#' RcppAlphahull
#'
#' @author Federico Airoldi \email{federico2.airoldi@@mail.polimi.it}
#'
#' @description
#' Voronoi tesselation/Delanuay triangulation, \eqn{\alpha}-shape and \eqn{\alpha}-hull computation using 
#' a C++ library.
#'
#' @details
#' RcppAlphahull is package aimed to compute Voronoi diagrams and Delanuay tesselations for sets of 
#' points using the C++ package \href{https://github.com/pvigier/FortuneAlgorithm}{MyGAL}. The package is
#' inspired by the R/Fortran package alphahull which does the same job, but much slower. Plot of some 
#' graphs are done by using this package functions. 
#'
#' @useDynLib RcppAlphahull
#' @import alphahull
#' @importFrom Rcpp evalCpp
#' @exportPattern "[[:alpha:]]+"
#'
#' @keywords Voronoi diagram, Delanuay tesselation
"_PACKAGE"
