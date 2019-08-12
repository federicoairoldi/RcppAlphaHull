# performs all the speed test scripts
print("PERFORMING SPEED TESTS")
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
for(test in c("ahull", "complement", "ashape", "delvor"))
  source(paste("speed_test_", test,".R",sep=""))
