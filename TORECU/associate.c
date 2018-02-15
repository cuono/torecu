#include "associate.h"

associate_t associate[associate_last_index + 1];

CU(associate)
{
  curetun(ssymbol);
  TT(&retun).symbol.s = TT(cuono(ttc.argv[0])).symbol.s;
  associate['z' - TT(&retun).symbol.s].value =
    *cuono(syono(ttc.argv[1]));
  return &retun;
}
RE(associate)
{
  reaffix2calan;
  if(0 == ttnchk(ttc.argv[0],ssymbol,stype))
    errxit("expected ssymbol,stype");
  recalan(snil,CUassociate);
}
TO(associate)
{
  totoken(saffix);
  toaffix(INFIX,1,0,REassociate);
}

/* cuono but symbol or no */
Stoken *syono(Stoken *token)
{
  if(tt.NUMBER & ssymbol)
    return &(associate['z' - tt.symbol.s].value);
  else
    return token;
}
