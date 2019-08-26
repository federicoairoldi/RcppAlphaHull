# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary script used to check features and correctness of the package RcppAlphahull

require(rbenchmark)
require(alphahull)
require(RcppAlphahull)

View(vorR$mesh)
View(vorcpp$mesh)

system.time(plot(vorR, col = c("blue", "red", "black", "blue"), pch = 19, wpoints = T, wlines = 'both', number = F))
system.time(RcppAlphahull::plot.delvor(vorcpp, col = c("blue", "red", "black", "blue"), pch = 19, 
                                       wpoints = T, wlines = 'both', number = F))

# plots in blue infinite edges
for(i in 1:dim(vorcpp$mesh)[1])
  lines(c(vorcpp$mesh[i, "mx1"], vorcpp$mesh[i, "mx2"]),
        c(vorcpp$mesh[i, "my1"], vorcpp$mesh[i, "my2"]),
        col = ifelse(vorcpp$mesh[i, "bp2"] == 1 | vorcpp$mesh[i, "bp1"] == 1, "blue", "red"))

n = 35 # 35
set.seed(353) # 353
x = runif(n)
y = runif(n)
theta = runif(300, 0, 2*pi)
r = runif(300, 0.2, 0.5)
x = r*cos(theta)+0.5
y = r*sin(theta)+0.5

system.time(RcppAlphahull::delvor(x,y))
system.time(alphahull::delvor(x,y))

vorcpp = RcppAlphahull::delvor(x,y)
vorR = alphahull::delvor(x, y)
alpha = 1
asR = alphahull::ashape(vorR, alpha = alpha)
ascpp = RcppAlphahull::ashape(vorcpp, alpha = alpha)
asR$alpha.extremes
ascpp$alpha.extremes
plot(asR, wpoints = T, wlines = 'both')
plot(ascpp, wpoints = T, col = c("red","black"))

alpha = 0.02
ahR = alphahull::ahull(vorR, alpha = alpha)
ahcpp = RcppAlphahull::ahull(vorcpp, alpha = alpha)
View(ahR$complement)
View(ahcpp$complement)
View(ahR$arcs)
View(ahcpp$arcs)
ahcpp$length
ahR$length
plot(ahR, col = c("red", "black", "black", "black", "black", "black"), asp = 1)
plot(ahcpp, col = c("cyan", "black", "black", "black", "black", "black"), asp = 1, add = T)
plot(ahR, col = c("red", "black", "black", "black", "black", "black"), asp = 1, add = T)
plot(vorR, number = F, wpoints = F, wlines = "vor", col = "grey", asp = 1)

# plotting arcs
wrow = which(ahR$arcs[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  # Sys.sleep(0.5)
  alphahull::arc(ahR$arcs[i,1:2], ahR$arcs[i,3], ahR$arcs[i,4:5], ahR$arcs[i,6], col = "red", lty = 2)
}
wrow = which(ahcpp$arcs[,3]>0)
for(i in wrow){
  # invisible(readline(prompt="Press [enter] to continue"))
  # Sys.sleep(0.5)
  alphahull::arc(ahcpp$arcs[i,1:2], ahcpp$arcs[i,3], ahcpp$arcs[i,4:5], ahcpp$arcs[i,6], col = "cyan", lty = 2)
}

# plotting arcs from complement
wrow = which(ahR$complement[,3]>0 & ahR$complement[,"ind"] == 1)
for(i in wrow){
  #invisible(readline(prompt="Press [enter] to continue"))
  #Sys.sleep(0.5)
  alphahull::arc(ahR$complement[i,1:2], ahR$complement[i,3], ahR$complement[i,17:18], ahR$complement[i,19], col = "red", lty = 2)
  points(ahR$complement[i,1],ahR$complement[i,2], pch = 19, col = "red")
}
wrow = which(ahcpp$complement[,3]>0)
for(i in wrow){
  #invisible(readline(prompt="Press [enter] to continue"))
  #Sys.sleep(0.5)
  alphahull::arc(ahcpp$complement[i,1:2], ahcpp$complement[i,3], ahcpp$complement[i,17:18], ahcpp$complement[i,19], col = "cyan", lty = 2)
  points(ahcpp$complement[i,1],ahcpp$complement[i,2], pch = 19, col = "cyan")
}

# plotting circles
wrow = which(ahR$complement[,3]>0)
for(i in wrow){
  #invisible(readline(prompt="Press [enter] to continue"))
  # Sys.sleep(0.5)
  alphahull::arc(ahR$complement[i,1:2], ahR$complement[i,3], c(0,1), pi, col = "red", lty = 2)
  points(ahR$complement[i,1],ahR$complement[i,2], pch = 19, col = "red")
}
wrow = which(ahcpp$complement[,3]>0)
for(i in wrow){
  #invisible(readline(prompt="Press [enter] to continue"))
  #Sys.sleep(0.5)
  alphahull::arc(ahcpp$complement[i,1:2], ahcpp$complement[i,3], c(0,1), pi, col = "cyan", lty = 2)
  points(ahcpp$complement[i,1],ahcpp$complement[i,2], pch = 19, col = "cyan")
}

alpha = 0.1
ahR = alphahull::ahull(x, y, alpha = alpha)
ahcpp = RcppAlphahull::ahull(x, y, alpha = alpha)

points(0.7, 0.8, pch = 19, col = "blue")
alphahull::inahull(ahR, c(0.7, 0.8))
RcppAlphahull::inahull(ahcpp, 0.7, 0.8)

system.time(alphahull::ahull(vorR, alpha = alpha))
system.time(RcppAlphahull::ahull(vorcpp, alpha = alpha))
system.time(alphahull::complement(vorR, alpha = alpha))
system.time(RcppAlphahull::complement(vorcpp, alpha = alpha))
system.time(alphahull::inahull(ahR, c(0.7, 0.8)))
system.time(RcppAlphahull::inahull(ahcpp, 0.7, 0.8))

# annulus sampling
n <- 300
theta<-runif(n,0,2*pi)
r<-sqrt(runif(n,0.25^2,0.5^2))
x<-cbind(0.5+r*cos(theta),0.5+r*sin(theta))
vorcpp = RcppAlphahull::delvor(x)
vorR = alphahull::delvor(x)
