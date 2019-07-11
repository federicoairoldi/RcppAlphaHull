library(Rcpp)
setwd("~/Documenti/ProgettoPACS")
sourceCpp("include/try.cpp")

View(vorR$mesh)
View(vorcpp$mesh)

plot(vorR, xlim = c(0,1), ylim = c(0,1))
for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]), 
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]), 
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

length(which(vorcpp$mesh[, "bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1))
length(which(vorR$mesh[, "bp2"] == 1 | vorR$mesh[, "bp1"] == 1))

system.time(computeVoronoiRcpp(x, y))
system.time(delvor(x, y))

set.seed(7658)
n.nodes = c( seq(10,90,by=10), seq(100,900,by=100), seq(1000,10000,by=1000) )
RTime = c()
CppTime = c()
for(n in n.nodes){
  print(paste("Simulation:",n))
  x = runif(n)
  y = runif(n)
  res = benchmark("Cpp" = computeVoronoiRcpp(x, y), "R" = delvor(x, y), replications = 1)
  CppTime = rbind(CppTime, res[which(res[,"test"] == "Cpp"), c("elapsed", "user.self", "sys.self")])
  RTime = rbind(RTime, res[which(res[,"test"] == "R"), c("elapsed", "user.self", "sys.self")])
}
tmp = cbind(CppTime, RTime)
x11()
matplot(n.nodes, tmp[,c(2,5)], type = "l", col = c("blue", "red"), lty = 1, 
        xlab = "Number of nodes", ylab = "CPU time")
lines(n.nodes, (0.000001)*n.nodes*log(n.nodes, 2), col = "green", lty = 2)
lines(n.nodes, (0.000000001)*n.nodes^2, col = "pink", lty = 2)
legend("topleft", legend = c("C++", "R", "nlogn"), fill = c("blue", "red", "green"), )




