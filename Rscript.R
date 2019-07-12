library(Rcpp)
setwd("~/Documenti/ProgettoPACS")
sourceCpp("include/try.cpp")

View(vorR$mesh)
View(vorcpp$mesh)

x11()
plot(vorR, col = c("blue", "red", "black", "blue"), pch = 19,
     xlim = c(0,1), ylim = c(0,1), wlines = 'both', number = T)
x11()
plot.delvor.cpp(vorcpp, col = c("blue", "red", "black", "blue"), pch = 19,
     xlim = c(0,1), ylim = c(0,1), wlines = 'both', number = T)

for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]), 
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]), 
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

length(which(vorcpp$mesh[, "bp2"] == 1 | vorcpp$mesh[, "bp1"] == 1))
length(which(vorR$mesh[, "bp2"] == 1 | vorR$mesh[, "bp1"] == 1))

system.time(computeVoronoiRcpp(x, y))
system.time(delvor(x, y))

n = 100
set.seed(309)
x = runif(n)
y = runif(n)

vorcpp = my.delvor(x,y)
vorR = delvor(x, y)

alpha = 0.05
asR = ashape(vorR, alpha = alpha)
plot(asR)
ascpp = my.ashape(vorcpp, alpha = alpha)
plot(ascpp)

ahR = ahull(asR, alpha = alpha)
plot(ahR)
ahcpp = my.ahull(ascpp, alpha = alpha)
plot(ahcpp)

system.time(ashape(vorR, alpha = alpha))
system.time(ashape(vorcpp, alpha = alpha))
