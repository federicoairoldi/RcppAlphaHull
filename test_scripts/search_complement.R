# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary function to search for discrepancies in the results of complement matrix
# this function checks in the matrix mesh there's a ball/halfplanes defined by c1, c2 and r
search_complement = function(c1, c2, r, mesh, esp){
  if(dim(mesh)[1] == 0)
    return(FALSE)
  for(i in 1:dim(mesh)[1]){
    if( abs(mesh[i,"c1"] - c1) < eps & abs(mesh[i,"c2"] - c2) < eps & abs(mesh[i,"r"] - r) < eps)
      return(TRUE)
  }
  return(FALSE)
}
