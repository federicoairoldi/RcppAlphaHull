# this script tests that my connection matrix retrieved from the MyGAL library has the
# same structure of the one returned by the "delvor" function of the package alphahull

# NB: this script is quite slow due to R inefficient allocation/problems with for
# cicles

require(alphahull)
require(Rcpp)
setwd("~/Documenti/ProgettoPACS")
sourceCpp("include/try.cpp") # compiling and loading the c++ function

# auxiliary function to search
search = function(ind1, ind2, mesh){
  for(i in 1:dim(mesh)[1]){
    if(mesh[i,"ind1"] == ind1 & mesh[i,"ind2"] == ind2)
      return(TRUE)
    if(mesh[i,"ind1"] == ind2 & mesh[i,"ind2"] == ind1)
      return(TRUE)
  }
  return(FALSE)
}

print("executing test")
# contains those test cases in which infinite edge of delvor result are not contained
# in the MyGAL result
infedges = c()  
# contains those test cases in which the number of the infinite edges of the two
# results are different
ninfedges = c() 
# contains those test cases in which finite edge of delvor result are not contained
# in the MyGAL result
finedges = c()  
# contains those test cases in which the number of the finite edges of the two
# results are different
nfinedges = c() 

n = 50 # number of point to sample for the voronoi diagram tests

for(i in 1:1000){
  print(i)
  set.seed(i) # setting the seed for the test
  x = runif(n) # sampling points
  y = runif(n)
  
  # calling the c++ and R function to compute voronoi diagram
  vorcpp = computeVoronoiRcpp(x, y)
  vorR = delvor(x,  y)
  
  # checking infinite edges
  # same number of infinite edges? if not adding the test to the queue ninfedges
  if(length(which(vorcpp$mesh[, "bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1))
     != length(which(vorR$mesh[, "bp2"] == 1 | vorR$mesh[, "bp1"] == 1)))
    ninfedges = c(ninfedges, i)
  
  # same infinite edges? if not adding the test to the queue infedges
  k = 0
  tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 1 | vorR$mesh[, "bp1"] == 1), 1:2]
  tmp2 = vorcpp$mesh[which(vorcpp$mesh[,"bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1), 1:2]
  for(i in 1:dim(tmp)[1])
    k = k + !(search(tmp[i,"ind1"], tmp[i,"ind2"], tmp2))
  
  if(k > 0)
    infedges = c(infedges, i)
  
  # checking finite edges
  # same number of finite edges? if not adding the test to the queue nfinedges
  if(length(which(vorcpp$mesh[, "bp2"] == 0 & vorcpp$mesh[, "bp1"] == 0))
     != length(which(vorR$mesh[, "bp2"] == 0 & vorR$mesh[, "bp1"] == 0)))
    nfinedges = c(nfinedges, 1)
  
  # same finite edges? if not adding the test to the queue finedges
  k = 0
  tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 0 & vorR$mesh[, "bp1"] == 0), 1:2]
  tmp2 = vorcpp$mesh[which(vorcpp$mesh[,"bp2"] == 0 & vorcpp$mesh[, "bp1"] == 0), 1:2]
  for(i in 1:dim(tmp)[1])
    k = k + !(search(tmp[i,"ind1"], tmp[i,"ind2"], tmp2))
  
  if(k > 0)
    finedges = c(finedges, i)
}

infedges
ninfedges
finedges
nfinedges

