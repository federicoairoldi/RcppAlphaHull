library(RcppAlphahull)
library(xtable)
library(rgl)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
load("Data_proxy.RData")

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

# x11()
# png("/home/federico/Dropbox/Alpha-hulls/Documenti/img/egg/egg_ahull.png", width = 960, height = 480 )
# par(mfrow = c(2,5))
print("ALPHA HULLS")
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

tab = c()
for(level in 1:levels)
  tab = rbind(tab, c(alpha.shapes[[level]]$delvor.obj$tri.obj$n, alpha.shapes[[level]]$length, alpha.hulls[[level]]$length))
colnames(tab) = c("Number of points", "alpha-shape length", "alpha-hull length")
rownames(tab) = 1:10

xtable(tab, label = "tab:test", digits = c(0, 0, rep(5,2)), caption = "main quantities of the eggshell analysis.")
