# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary script used to check features and correctness of the package RcppAlphahull

require(alphahull)
require(RcppAlphahull)

View(vorR$mesh)
View(vorcpp$mesh)

system.time(plot(vorR, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = F,
                 xlim = c(0,1), ylim = c(0,1), wlines = 'both', number = T))
system.time(RcppAlphahull::plot.delvor(vorcpp, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = F,
                            xlim = c(0,1), ylim = c(0,1), wlines = 'vor', number = T))

for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]),
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]),
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

n = 20
set.seed(n)
x = runif(n)
y = runif(n)

vorcpp = RcppAlphahull::delvor(x,y)
vorR = alphahull::delvor(x, y)
alpha = 1.2
asR = alphahull::ashape(vorR, alpha = alpha)
ascpp = RcppAlphahull::ashape(vorcpp, alpha = alpha)
asR$alpha.extremes
ascpp$alpha.extremes
View(asR$edges)
View(ascpp$edges)
plot(asR, wpoints = T)
plot(ascpp, wpoints = T)

alpha = 2
ahR = alphahull::ahull(x, y, alpha = alpha)
plot(ahR, do.shape = T, col = c("red", "pink", "black", "cyan", "blue", "black"), wlines = "n", asp = 1)
warcs = which(ahR$arcs[,3]>0)
for(i in warcs)
  alphahull::arc(ahR$arcs[i,1:2], ahR$arcs[i,3], c(0,1), pi, col = "grey", lty = 2)

wrow = which(ahR$complement[,3]>0)
for(i in wrow)
  alphahull::arc(ahR$complement[i,1:2], ahR$complement[i,3], c(0,1), pi, col = "grey", lty = 2)

ahcpp = RcppAlphahull::ahull(ascpp, alpha = alpha)
plot(ascpp)

system.time(alphahull::ahull(x, y, alpha = alpha))
system.time(RcppAlphahull::ahull(vorcpp, alpha = alpha))
