library(Rcpp)
setwd("~/Documenti/ProgettoPACS")
sourceCpp("include/try.cpp")

View(vorR$mesh)
View(vorcpp$mesh)

plot(vorR, xlim = vorcpp$temp[1:2], ylim = vorcpp$temp[3:4])
for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]), 
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]), 
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

length(which(vorcpp$mesh[, "bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1))
length(which(vorR$mesh[, "bp2"] == 1 | vorR$mesh[, "bp1"] == 1))

tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 1),]
for(i in 1:dim(tmp)[1])
  lines(c(tmp[i, "mx1"], tmp[i, "mx2"]), 
        c(tmp[i, "my1"], tmp[i, "my2"]), col = "green")

