# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# this script tests that my connection matrix retrieved from the MyGAL library has the
# same structure of the one returned by the "delvor" function of the package alphahull
#
# parameters that can be modified:
# - n:                 number of sites
# - n.test:            number of tests to be excecuted
# - set.seed(rule(i)): seed for the i-th test, one can assign any rule for the seed used to sample the
#                      points. rule(i) is a function that returns a number (even a floating point one)
#
# NB: this script is quite slow due to R inefficient allocation/problems with for cicles

require(alphahull)
require(RcppAlphahull)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
source("search.R")

print("executing test")
# contains those test cases in which infinite edge of delvor result are not contained
# in the MyGAL result
infedges = c()
# contains those test cases in which the number of the infinite edges of the two
# results are different
n.infedges = c()
# contains those test cases in which finite edge of delvor result are not contained
# in the MyGAL result
finedges = c()
# contains those test cases in which the number of the finite edges of the two
# results are different
n.finedges = c()

n = 50 # number of point to sample for the voronoi diagram tests (feel free to change)
n.test = 200
used.n = c() # keeps track of the number of points sampled for the different tests

for(i in 1:n.test){
  if(i%%50==0) print(i) # print the number of the test on the command line (can be commented to save some time)
  set.seed(i)  # setting the seed for the test (feel free to modify and set your own seed)
  n = sample(50:100, 1)
  x = runif(n) # sampling points
  y = runif(n)

  # calling the RcppAlphahull and alphahull function to compute Voronoi diagram
  vorcpp = RcppAlphahull::delvor(x,y)
  vorR = alphahull::delvor(x,y)

  # checking infinite edges
  # same number of infinite edges? if not adding the test to the queue n.infedges
  if(length(which(vorcpp$mesh[, "bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1))
     != length(which(vorR$mesh[, "bp2"] == 1 | vorR$mesh[, "bp1"] == 1)))
    n.infedges = c(n.infedges, i)

  # same infinite edges? if not adding the test to the queue infedges
  k = 0
  tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 1 | vorR$mesh[, "bp1"] == 1), 1:2]
  tmp2 = vorcpp$mesh[which(vorcpp$mesh[,"bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1), 1:2]
  for(i in 1:dim(tmp)[1])
    k = k + !(search(tmp[i,"ind1"], tmp[i,"ind2"], tmp2))

  if(k > 0)
    infedges = c(infedges, i)

  # checking finite edges
  # same number of finite edges? if not adding the test to the queue n.finedges
  if(length(which(vorcpp$mesh[, "bp2"] == 0 & vorcpp$mesh[, "bp1"] == 0))
     != length(which(vorR$mesh[, "bp2"] == 0 & vorR$mesh[, "bp1"] == 0)))
    n.finedges = c(n.finedges, i)

  # same finite edges? if not adding the test to the queue finedges
  k = 0
  tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 0 & vorR$mesh[, "bp1"] == 0), 1:2]
  tmp2 = vorcpp$mesh[which(vorcpp$mesh[,"bp2"] == 0 & vorcpp$mesh[, "bp1"] == 0), 1:2]
  for(i in 1:dim(tmp)[1])
    k = k + !(search(tmp[i,"ind1"], tmp[i,"ind2"], tmp2))

  if(k > 0)
    finedges = c(finedges, i)
  
  used.n = c(used.n, n)
}

rm(list = c("k", "tmp", "tmp2", "i", "vorcpp", "vorR", "x", "y"))

infedges
n.infedges
finedges
n.finedges
