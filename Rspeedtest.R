# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# this script tests the performances of the package alphahull with respect to the
# performances of the R/C++ hybrid package RcppAlphahull
#
# by setting a different number of sites the scripr compute the Voronoi diagram and
# Delanuay tesselation of n randomized point of R2 in [0;1]x[0;1] (if one likes, he
# can change the seed)

require(alphahull)
require(RcppAlphahull)

set.seed(7658)
n.nodes = c( seq(100,900,by=100),
             seq(1000,9000,by=1000),
             seq(10000,50000,by=10000))
relative = c()
RTime = c()
CppTime = c()
for(n in n.nodes){
  print(paste("Simulation:",n))
  x = runif(n)
  y = runif(n)
  res = benchmark("Cpp" = mydelvor(x, y), "R" = delvor(x, y), replications = 1)
  CppTime = rbind(CppTime, res[which(res[,"test"] == "Cpp"), c("elapsed", "user.self", "sys.self")])
  RTime = rbind(RTime, res[which(res[,"test"] == "R"), c("elapsed", "user.self", "sys.self")])
  relative = c(relative, res[which(res[,"test"] == "R"), c("relative")])
}
tmp = cbind(CppTime, RTime)


Cpp.User = tmp[,2]
R.User = tmp[,5]
transfCpp = n.nodes*log(n.nodes, 2)
transfR = (n.nodes*log(n.nodes, 2))^2
fit.cpp = lm(Cpp.User ~ 0 + transfCpp)
summary(fit.cpp)
fit.R = lm(R.User ~ 0 + transfR)
summary(fit.R)

x11()
matplot(n.nodes, cbind(Cpp.User, R.User), type = "l", col = c("blue", "red"), lty = 1,
        xlab = "Number of nodes", ylab = "CPU time")
lines(n.nodes, fit.cpp$fitted.values, col = "green", lty = 2)
lines(n.nodes, fit.R$fitted.values, col = "orange", lty = 2)
legend("topleft", legend = c("C++", "R", "nlogn"), fill = c("blue", "red", "green"), )

R.User/Cpp.User
