# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary script used to check features and correctness of the package RcppAlphahull

require(alphahull)
require(RcppAlphahull)

View(vorR$mesh)
View(vorcpp$mesh)

system.time(plot(vorR, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = F,
                 xlim = c(0,1), ylim = c(0,1), wlines = 'both', number = T))
system.time(plot.delvor.cpp(vorcpp, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = F,
                            xlim = c(0,1), ylim = c(0,1), wlines = 'both', number = T))

for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]),
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]),
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

n = 10
set.seed(309)
x = runif(n)
y = runif(n)

vorcpp = my.delvor(x,y)
vorR = delvor(x, y)
alpha = 3
asR = ashape(vorR, alpha = alpha)
ascpp = my.ashape(vorcpp, alpha = alpha)
asR$alpha.extremes
ascpp$alpha.extremes

ahR = ahull(vorR, alpha = alpha)
plot(asR)
ahcpp = my.ahull(ascpp, alpha = alpha)
plot(ahcpp)

system.time(ashape(vorR, alpha = alpha))
system.time(my.ashape(vorcpp, alpha = alpha))
