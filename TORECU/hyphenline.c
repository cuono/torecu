#include "hyphenline.h"
#include "xy.h"

CU(hyphenline)
{
}
RE(hyphenline)
{
  
}
TO(hyphenline)
{
  totoken(saffix); xy;
  toaffix(NOFIX,1,0,REhyphenline);
  tomatch; puts(match);
}
