#include "symbol.h"
#include "xy.h"

TO(symbol)
{
  totoken(ssymbol | stype);
  tt.symbol.s = *to->p;
  xy;
  printf("%d,%d\n",X,Y);
}
