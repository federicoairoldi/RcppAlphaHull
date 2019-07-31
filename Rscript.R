# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary script used to check features and correctness of the package RcppAlphahull

require(alphahull)
require(RcppAlphahull)

View(vorR$mesh)
View(vorcpp$mesh)

system.time(plot(vorR, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = F,
                 xlim = c(0,1), ylim = c(0,1), wlines = 'vor', number = T))
system.time(RcppAlphahull::plot.delvor(vorcpp, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = T,
                                       xlim = c(0,1), ylim = c(0,1), wlines = 'vor', number = F))

for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]),
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]),
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

n = 10
set.seed(353)
x = runif(n)
y = runif(n)

vorcpp = RcppAlphahull::delvor(x,y)
vorR = alphahull::delvor(x, y)
alpha = 0.04
asR = alphahull::ashape(vorR, alpha = alpha)
ascpp = RcppAlphahull::ashape(vorcpp, alpha = alpha)
asR$alpha.extremes
ascpp$alpha.extremes
View(asR$edges)
View(ascpp$edges)
plot(asR, wpoints = T)
plot(ascpp, wpoints = T)

ahR = alphahull::ahull(x, y, alpha = alpha)
ahcpp = RcppAlphahull::ahull(x, y, alpha = alpha)
View(ahR$complement)
View(ahcpp$complement)

alpha = 1
ahR = alphahull::ahull(x, y, alpha = alpha)
ahcpp = RcppAlphahull::ahull(x, y, alpha = alpha)
plot(ahR, do.shape = T, col = c("red", "pink", "black", "cyan", "blue", "black"), wlines = "n", asp = 1,
     xlim = c(-0.5,1), ylim = c(0,1.1), wpoints = F, number = T)
warcs = which(ahR$arcs[,3]>0)
for(i in warcs)
  alphahull::arc(ahR$arcs[i,1:2], ahR$arcs[i,3], c(0,1), pi, col = "grey", lty = 2)

# plotting arcs
wrow = which(ahR$complement[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  alphahull::arc(ahR$complement[i,1:2], ahR$complement[i,3], ahR$complement[i,17:18], ahR$complement[i,19], col = "red", lty = 2)
}
wrow = which(ahcpp$complement[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  alphahull::arc(ahcpp$complement[i,1:2], ahcpp$complement[i,3], ahcpp$complement[i,17:18], ahcpp$complement[i,19], col = "cyan", lty = 2)
}

# plotting circles
wrow = which(ahR$complement[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  alphahull::arc(ahR$complement[i,1:2], ahR$complement[i,3], c(0,1), pi, col = "red", lty = 2)
}
wrow = which(ahcpp$complement[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  alphahull::arc(ahcpp$complement[i,1:2], ahcpp$complement[i,3], c(0,1), pi, col = "cyan", lty = 2)
}

plot(ascpp)
plot(ahcpp)

system.time(alphahull::ahull(x, y, alpha = alpha))
system.time(RcppAlphahull::ahull(vorcpp, alpha = alpha))
