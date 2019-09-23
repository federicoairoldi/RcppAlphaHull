# performs all the speed test scripts
print("PERFORMING SPEED TESTS")
dfcpp = c()
dfR = c()
func = c("delvor", "ashape", "complement", "ahull")
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
for(test in func){
  source(paste("speed_test_", test,".R",sep=""))
  
  dfcpp = cbind(dfcpp, Cpp.User)
  dfR = cbind(dfR, R.User)
}

colnames(dfcpp) = func
colnames(dfR) = func
df = c(seq(100, 1000, by = 100))
df = cbind(df, dfcpp, dfR)
colnames(df) = c("n", func, func)
print(xtable(df, label = "tab:speed", caption = "speed comparison of the two packages.", digits = c(0, 0, rep(3,8))),
      include.rownames = F)
