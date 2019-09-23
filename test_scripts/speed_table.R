#
# this script tests the performances of the package alphahull with respect to the
# performances of the R/C++ hybrid package RcppAlphahull

require(rbenchmark)
require(xtable)
require(alphahull)
require(RcppAlphahull)

set.seed(3)
n.nodes = c(10, 100, 1000, 10000)
times = c()

for(n in n.nodes){
  print(paste("Simulation:",n))
  x = runif(n)
  y = runif(n)
  alpha = runif(1)
  vorcpp = RcppAlphahull::delvor(x, y)
  vorR = alphahull::delvor(x, y)
  res = benchmark("delvorCpp" = RcppAlphahull::delvor(x, y), 
                  "delvorR" = alphahull::delvor(x, y),
                  "ashapeCpp" = RcppAlphahull::ashape(vorcpp, alpha = alpha),
                  "ashapeR" = alphahull::ashape(vorR, alpha = alpha),
                  "complementCpp" = RcppAlphahull::complement(vorcpp, alpha = alpha),
                  "complementR" = alphahull::complement(vorR, alpha = alpha),
                  "ahullCpp" = RcppAlphahull::ahull(vorcpp, alpha = alpha),
                  "ahullR" = alphahull::ahull(vorR, alpha = alpha),
                  replications = 1)
  times = rbind(times, c(n, res[which(res[,"test"]=="delvorCpp"), "user.self"], 
                            res[which(res[,"test"]=="ashapeCpp"), "user.self"], 
                            res[which(res[,"test"]=="complementCpp"), "user.self"],
                            res[which(res[,"test"]=="ahullCpp"), "user.self"],
                            res[which(res[,"test"]=="delvorR"), "user.self"], 
                            res[which(res[,"test"]=="ashapeR"), "user.self"],
                            res[which(res[,"test"]=="complementR"), "user.self"],
                            res[which(res[,"test"]=="ahullR"), "user.self"]))
}
colnames(times) = c("n", "delvor", "ashape", "complement", "ahull", "delvor", "ashape", "complement", "ahull")
times
print(xtable(times, label = "tab:speed", digits = c(0, 0, rep(3,8))), include.rownames = F)

      