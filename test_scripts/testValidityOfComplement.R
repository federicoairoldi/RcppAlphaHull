# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# this script tests that my complement object retrieved starting from a delvor object has the same
# structure of the one returned by the "ahull" function of the package alphahull
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
# contains those test cases in which the complement matrix don't coincide
not.matching.length = c() # different number of components
not.matching.planes = c() # different planes
not.matching.balls = c() # different balls

n.test = 1000
used.n = c() # keeps track of the number of points sampled for the different tests

for(i in 1:n.test){
  if(i%%50==0) print(i) # print the number of the test on the command line (can be commented to save some time)
  set.seed(i)  # setting the seed for the test (feel free to modify and set your own seed)
  n = sample(50:300, 1)
  x = runif(n,0,10) # sampling points
  y = runif(n,0,10)
  
  alpha = runif(1); # sampling alpha
  
  # calling the RcppAlphahull and alphahull function to compute the alpha shape
  complementcpp = RcppAlphahull::complement(x,y,alpha)
  complementR = alphahull::complement(x,y,alpha)
  
  # check lengths
  # do the alpha shapes have the same length? if not adding the test to the queue not.matching.length
  if( dim(complementcpp)[1]!=dim(complementR)[1] )
    not.matching.length = c(not.matching.length, 1)
  
  k = 0
  for(type in c(-1, -2, -3, -4)){
    planesR = complementR[which(complementR[,"r"] == type),]
    planescpp = complementcpp[which(complementcpp[,"r"] == type),]
    if(dim(planesR)[1]>0)
      for(j in dim(planesR)[1])
        k = k + !search(planesR[j,"ind1"], planesR[j,"ind2"], planescpp)
  }
  
  if(k > 0)
    not.matching.planes = c(not.matching.planes, i)
  
  k = 0
  ballsR = complementR[which(complementR[,"r"] > 0),]
  ballscpp = complementcpp[which(complementcpp[,"r"] > 0),]
  for(i in dim(ballsR)[1])
    k = k + !search(ballsR[i,"ind1"], ballsR[i,"ind2"], ballscpp)
  
  if(k > 0)
    not.matching.balls = c(not.matching.balls, i)
  
  used.n = c(used.n, n)
}

rm(list = c("k", "i", "complementR", "complementcpp", "x", "y"))

not.matching.length
not.matching.planes
not.matching.balls

