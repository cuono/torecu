#include "mul.h"

CU(mul_infix)
{
  curetun(snumber | stype);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n
    * TT(cuono(syono(ttc.argv[1]))).number.n;
  return &retun;
}
/*
CU(mul_prefix)
{
  curetun(snumber | stype);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n;
  return &retun;
  }*/
RE(mul)
{
  reicanp(snumber|ssymbol);
  reaffix2calan;
  if(ttc.argc == 2){
    recalan(ssymbol|snumber|stype,CUmul_infix);
  }/*
  else{
    recalan(ssymbol|snumber|stype,CUmul_prefix);
    }*/
}
TO(mul)
{
  totoken(saffix);
  toaffix(INFIX,1,2,REmul);
}
