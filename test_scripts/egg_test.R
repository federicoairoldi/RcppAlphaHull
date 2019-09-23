library(RcppAlphahull)
library(xtable)
library(rgl)
library(pracma)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
load("Data_proxy.RData")

#### SECTIONS' COMPUTATIONS ####
alpha.shapes = list()
alpha.hulls = list()
levels = length(unique(data_proxy[,"label"]))
colors = c("blue", "yellow", "green", "orange", "spring green", "pink", "grey", "light green", "steel blue", "turquoise1")

r3dDefaults$windowRect = c(0,50,800,800)
plot3d(data_proxy[,1:3], col = colors[data_proxy[,"label"]], pch = 19)
rgl.postscript("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/egg_planes.pdf", "pdf")

# x11()
# png("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/egg_ashape.png", width = 960, height = 480 )
# par(mfrow = c(2,5))
print("ALPHA SHAPES")
tic()
for(level in 1:levels){
  xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
  
  alpha.shape = RcppAlphahull::ashape(xy, alpha = 0.85)
  # png(paste("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/face",level,"_ashape.png",sep=""), width = 960, height = 480 )
  # plot(alpha.shape, col = c("red", col = colors[level]), asp = 1, xlab = "", ylab = "", pch = 19, main = paste("Section",level), cex.main = 3)
  # graphics.off()
  alpha.shapes[[level]] = alpha.shape
  print(paste("Face ",level,": length = ",alpha.shape$length,sep = ""))
}
# graphics.off()
t1 = toc()

# x11()
# png("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/egg_ahull.png", width = 960, height = 480 )
# par(mfrow = c(2,5))
print("ALPHA HULLS")
tic()
for(level in 1:levels){
  xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
  
  alpha.hull = RcppAlphahull::ahull(xy, alpha = 0.85)
  # png(paste("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/face",level,"_ahull.png",sep=""), width = 960, height = 480 )
  # plot(alpha.hull, col = c("red", "white", col = colors[level]), asp = 1, xlab = "", ylab = "", pch = 19, main = paste("Section",level), cex.main = 3)
  # graphics.off()
  alpha.hulls[[level]] = alpha.hull
  print(paste("Face ",level,": length = ",alpha.hull$length,sep = ""))
}
# graphics.off()
t2 = toc()

tab = c()
for(level in 1:levels)
  tab = rbind(tab, c(alpha.shapes[[level]]$delvor.obj$tri.obj$n, alpha.shapes[[level]]$length, alpha.hulls[[level]]$length))
colnames(tab) = c("Number of points", "alpha-shape length", "alpha-hull length")
rownames(tab) = 1:10

xtable(tab, label = "tab:egg_test", digits = c(0, 0, rep(5,2)), caption = "main quantities of the eggshell analysis.")



#### SPEED TEST ####
# alphahull vs RcppAlphahull speed
funcs = list()
funcs[["RcppAlphahull"]] = list("ashape" = RcppAlphahull::ashape, "ahull" = RcppAlphahull::ahull)
funcs[["alphahull"]] = list("ashape" = alphahull::ashape, "ahull" = alphahull::ahull)

df = data.frame("fun" = c("ashape", "ahull", "total"))

for(package in funcs){
  tic()
  for(level in 1:levels){
    xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
    alpha.shape = package$ashape(xy, alpha = 0.85)
    alpha.shapes[[level]] = alpha.shape
  }
  t1 = toc()
  
  tic()
  for(level in 1:levels){
    xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
    alpha.hull = package$ahull(xy, alpha = 0.85)
    alpha.hulls[[level]] = alpha.hull
  }
  t2 = toc()
  
  df = cbind(df, package = c(t1,t2, t1+t2))
}

colnames(df) = c("", "RcppAlphahull", "alphahull")
df 

xtable(df, label = "tab:egg_speed", digits = c(0, 0, rep(5,2)), 
       caption = "eggshell analysis speed comparison between \texttt{alphahull} and \texttt{RcppAlphahull}.")

