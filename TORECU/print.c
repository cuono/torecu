#include "print.h"

CU(print)
{
  if(TT(cuono(ttc.argv[0])).NUMBER & snumber)
    printf("%d\n",TT(cuono(ttc.argv[0])).number.n);
  if(TT(cuono(ttc.argv[0])).NUMBER & ssymbol)
    printf("%d\n",TT(cuono(syono(ttc.argv[0]))).number.n);
  if(TT(cuono(ttc.argv[0])).NUMBER & sstring)
    printf("%s\n",TT(cuono(syono(ttc.argv[0]))).string.s);
}
RE(print)
{
  reaffix2calan;
  if(!(TT(cuono(ttc.argv[0])).NUMBER & stype))
    errxit("non-type passed to PRINT");
  recalan(snil,CUprint);
}
TO(print)
{
  totoken(saffix);
  toaffix(PREFIX,1,0,REprint);
}
