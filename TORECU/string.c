#include "string.h"

TO(string)
{
  totoken(sstring | stype);
  char *P = to->p + 1;
  while(*(++to->p) != '"')
    {
      if(*(to->p) == '\0')
	{
	  errxit("string missing quote");
	}
    }
  int len = to->p -P;
  tt.string.s = malloc(len);
  strncpy(tt.string.s,P,len);
  tt.string.s[len] = '\0';
  //printf("STRING; %s\n",tt.string.s);
  ++to->p;
}
