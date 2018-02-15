#include "div.h"

CU(div_infix)
{
  curetun(snumber | stype);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n
    / TT(cuono(syono(ttc.argv[1]))).number.n;
  return &retun;
}
/*
CU(div_prefix)
{
  curetun(snumber | stype);
  TT(&retun).number.n = TT(cuono(syono(ttc.argv[0]))).number.n;
  return &retun;
  }*/
RE(div)
{
  reicanp(snumber|ssymbol);
  reaffix2calan;
  if(ttc.argc == 2){
    recalan(ssymbol|snumber|stype,CUdiv_infix);
  }/*
  else{
    recalan(ssymbol|snumber|stype,CUdiv_prefix);
    }*/
}
TO(div)
{
  totoken(saffix);
  toaffix(INFIX,1,2,REdiv);
}
