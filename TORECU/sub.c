#include "sub.h"

CU(sub_infix)
{
  static Stoken RETU;
  TT(&RETU).NUMBER = snumber | stype;
  TT(&RETU).number.n = TT(cuono(syono(ttc.argv[0]))).number.n
    - TT(cuono(syono(ttc.argv[1]))).number.n;
  return &RETU;
}
CU(sub_prefix)
{
  static Stoken RETU;
  TT(&RETU).NUMBER = snumber | stype;
  TT(&RETU).number.n = -TT(cuono(syono(ttc.argv[0]))).number.n;
  return &RETU;
}
RE(sub)
{
  reicanp(snumber|ssymbol);
  reaffix2calan;
  if(ttc.argc == 2){
    recalan(ssymbol|snumber|stype,CUsub_infix);
  }
  else{
    recalan(ssymbol|snumber|stype,CUsub_prefix);
  }
}
TO(sub)
{
  totoken(saffix);
  toaffix(INFIX,1,1,REsub);
}
