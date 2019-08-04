# Federico Airoldi    matricola: 892377   codice persona: 10484065
#
# auxiliary function to search for discrepancies in the results
search = function(ind1, ind2, mesh){
  if(dim(mesh)[1] == 0)
    return(FALSE)
  for(i in 1:dim(mesh)[1]){
    if(mesh[i,"ind1"] == ind1 & mesh[i,"ind2"] == ind2)
      return(TRUE)
    if(mesh[i,"ind1"] == ind2 & mesh[i,"ind2"] == ind1)
      return(TRUE)
  }
  return(FALSE)
}
