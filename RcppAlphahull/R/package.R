#' RcppAlphahull
#'
#' @author Federico Airoldi \email{federico2.airoldi@@mail.polimi.it}
#'
#' @description
#' RcppAlphahull is package aimed to compute Voronoi diagrams and Delanuay tesselations
#' for sets of points using the C++ package MyGAL (link...). The package is based on the
#' R/Fortran package alphahull which does the same job, but much slower.
#'
#' @details
#' devo aggiungere i dettagli
#' 
#' 
#'
#' @useDynLib RcppAlphahull
#' @import alphahull
#' @importFrom Rcpp evalCpp
#' @exportPattern "[[:alpha:]]+"
#'
#' @keywords Voronoi diagram, Delanuay tesselation
"_PACKAGE"
