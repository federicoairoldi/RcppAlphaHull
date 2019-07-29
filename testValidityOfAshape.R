# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# this script tests that my alpha-shape object retrieved starting from a delvor objectt has the same
# structure of the one returned by the "ashape" function of the package alphahull
#
# parameters that can be modified:
# - n:                 number of sites
# - n.test:            number of tests to be excecuted
# - alpha:             parameter for alpha shape computation (may be fixed for each test)
# - set.seed(rule(i)): seed for the i-th test, one can assign any rule for the seed used to sample the
#                      points. rule(i) is a function that returns a number (even a floating point one)
#
# NB: this script is quite slow due to R inefficient allocation/problems with for cicles

require(alphahull)
require(RcppAlphahull)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
source("search.R")

print("executing test")
# contains those test cases in which the ashape matrice don't coincide
not.matching = c()
not.matching.size = c()
not.matching.length = c()

perc.length.diff = c()
eps = 1e-15 # tollerance for confrontation of alpha shape lengths
# up to 1e-15 lengths are the "same"

n = 50 # number of point to sample for the voronoi diagram tests (feel free to change)
n.test = 1000

for(i in 1:n.test){
  if(i%%50==0) print(i) # print the number of the test on the command line (can be commented to save some time)
  set.seed(i)  # setting the seed for the test (feel free to modify and set your own seed)
  x = runif(n,0,10) # sampling points
  y = runif(n,0,10)

  alpha = runif(1); # sampling alpha

  # calling the RcppAlphahull and alphahull function to compute the alpha shape
  ascpp = RcppAlphahull::ashape(x,y,alpha)
  asR = alphahull::ashape(x,y,alpha)

  # check lengths
  # do the alpha shapes have the same length? if not adding the test to the queue not.matching.length
  if( dim(asR$edges)[1]!=0 )
    perc.length.diff = c(perc.length.diff, abs( (asR$length - ascpp$length)/ascpp$length ))
  else
    perc.length.diff = c(perc.length.diff, 0)

  # checking edges
  # same number of edges? if not adding the test to the queue not.matching.size
  if(dim(asR$edges)[1]!=dim(ascpp$edges)[1])
    not.matching.size = c(not.matching.size, i)

  # same edges for the alpha? if not adding the test to the queue of wrong tests
  k = 0
  if(dim(asR$edges)[1]!=0)
    for(i in 1:dim(asR$edges)[1])
      k = k + !(search(asR$edges[i,"ind1"], asR$edges[i,"ind2"], ascpp$edges))

  if(k > 0)
    not.matching = c(not.matching, i)
}

not.matching.length = which(perc.length.diff >= eps)

rm(list = c("k", "i", "ascpp", "asR", "x", "y"))

not.matching.size
not.matching.length
not.matching

