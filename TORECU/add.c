#include "add.h"

CU(add_infix)
{
  curetun(snumber | stype);
  printf("%d + %d\n",TT(cuono(syono(ttc.argv[0]))).number.n,
	 TT(cuono(syono(ttc.argv[1]))).number.n);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n
    + TT(cuono(syono(ttc.argv[1]))).number.n;
  return &retun;
}
CU(add_prefix)
{
  curetun(snumber | stype);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n;
  return &retun;
}
RE(add)
{
  reicanp(snumber|ssymbol);
  reaffix2calan;
  if(ttc.argc == 2){
    recalan(ssymbol|snumber|stype,CUadd_infix);
  }
  else{
    recalan(ssymbol|snumber|stype,CUadd_prefix);
  }
}
TO(add)
{
  totoken(saffix);
  toaffix(INFIX,1,1,REadd);
}
