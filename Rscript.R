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

search = function(ind1, ind2, mesh){
  for(i in 1:dim(mesh)[1]){
      if(mesh[i,"ind1"] == ind1 & mesh[i,"ind2"] == ind2)
        return(TRUE)
      if(mesh[i,"ind1"] == ind2 & mesh[i,"ind2"] == ind1)
        return(TRUE)
    }
  return(FALSE)
}

k = 0
tmp = vorR$mesh[which(vorR$mesh[,"bp2"] == 1 | vorR$mesh[, "bp1"] == 1), 1:2]
tmp2 = vorcpp$mesh[which(vorcpp$mesh[,"bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1), 1:2]
for(i in 1:dim(tmp)[1])
  k = k + !(search(tmp[i,"ind1"], tmp[i,"ind2"], tmp2))


