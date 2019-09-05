library(RcppAlphahull)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
load("Data_proxy.RData")

lista = list()


levels = length(unique(data_proxy[,"label"]))
x11()
par(mfrow = c(2,5))
for(level in 1:levels){
  xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
  
  alpha.shape = RcppAlphahull::ashape(xy, alpha = 0.5)
  # plot(alpha.shape, col = c("blue","black"))
  lista = list(lista, alpha.shape)
}


levels = length(unique(data_proxy[,"label"]))
x11()
par(mfrow = c(2,5))
for(level in 1:levels){
  xy = princomp(data_proxy[which(data_proxy[,"label"]==level), 1:3], scores = T)$scores[,1:2]
  
  alpha.hull = RcppAlphahull::ahull(xy, alpha = 0.5)
  # plot(alpha.shape, col = c("blue","black"))
  lista = list(lista, alpha.hull)
}
