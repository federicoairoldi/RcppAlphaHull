library(Rcpp)
setwd("~/Documenti/ProgettoPACS")
sourceCpp("include/try.cpp")

set.seed(7658)
n.nodes = c( seq(10,90,by=10), 
             seq(100,900,by=100), 
             seq(1000,9000,by=1000)
             )
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
