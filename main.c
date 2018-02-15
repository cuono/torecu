#include "miscellaneous.h" //errxit
#include "tokenizer.h"
#include "retokenizer.h"
#include "cutenizer.h"
#include <stdlib.h> //malloc
#include <stdio.h>  //printf
#include <string.h> //strcpy

Stok_dclr tok_dclr[]; int len_tok_dclr;

/*\\\\\\\\\\\\\\\\\\\\\
  CONFIG TO PROGRAMMER
  \\\\\\\\\\\\\\\\\\\\\*/

/* add/remove TORECU priorto tok_dclr[] */
#include "TORECU/associate.h"
#include "TORECU/wav.h"
#include "TORECU/print.h"
#include "TORECU/blank.h"
#include "TORECU/comment.h"
#include "TORECU/newline.h"
#include "TORECU/parentheses.h"
#include "TORECU/string.h"
#include "TORECU/symbol.h"
#include "TORECU/number.h"
#include "TORECU/add.h"
#include "TORECU/sub.h"
#include "TORECU/mul.h"
#include "TORECU/div.h"
#include "TORECU/hyphenline.h"

/* declaration of TO */
Stok_dclr tok_dclr[]={
  {.regex="+",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOadd},
  {.regex="_",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOsub},  
  {.regex="*",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOmul},
  {.regex="/",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOdiv},      
  {.regex="-+",
   .isreg=1,
   .level=0,
   .length=0,
   .tokenizer=TOhyphenline},
  {.regex="wav",
   .isreg=0,
   .level=0,
   .length=0,
   .tokenizer=TOwav},
  {.regex="=",
   .isreg=0,
   .level=0,
   .length=0,
   .tokenizer=TOassociate},
  {.regex="print",
   .isreg=0,
   .level=0,
   .length=0,
   .tokenizer=TOprint},
  {.regex=" ",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOblank},
  {.regex="#",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOcomment},
  {.regex="\n+",
   .isreg=1,
   .level=0,
   .length=1,
   .tokenizer=TOnewline}, /* do not compete and do not meet competition */
  {.regex="a-z", /* "a-z+" */
   .isreg=1,
   .level=1,
   .length=1,    /* LONGEST */
   .tokenizer=TOsymbol},
  {.regex="0-9+",
   .isreg=1,
   .level=1,
   .length=LONGEST,
   .tokenizer=TOnumber},
  {.regex="\"",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOstring},
  {.regex=")",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOparenR},
  {.regex="(",
   .isreg=0,
   .level=0,
   .length=1,
   .tokenizer=TOparenL},
};int len_tok_dclr = sizeof(tok_dclr)/sizeof(Stok_dclr);


#define L l->type.affix.type
#define R r->type.affix.type
/* precedence and logic of L and R */
LorR // RETURN of 1 is R-side was retokenized in LorR
     // RETURN of 0 is L-side can retokenize after return
     // solve of R-side is R->type.affix.retokenizer and return of 1
     // solve of L-side is return 0
{
  // case for - or + as PREFIX not INFIX
  if(L == PREFIX && (l->type.affix.retokenizer == REadd
  		     || l->type.affix.retokenizer == REsub))
    {
      //puts("LorR L == PREFIX and REadd REsub");
      return 0;
    }
  if(L == PREFIX && R == INFIX)
    {
      r->type.affix.retokenizer(r);
      return 1;
    }
  // case for precedence for INFIX
  if(L == INFIX && R == INFIX)
    {
      if(l->type.affix.precedence < r->type.affix.precedence)
	{
	  r->type.affix.retokenizer(r);
	  return 1;
	}
    }
  return 0;
}
#undef R
#undef L

/*\\\\\\\\\\\\\\\\\\\\\
  CONFIG TO PROGRAMMER END
  \\\\\\\\\\\\\\\\\\\\\*/

int main()
{
  // text -- remember to terminate with \n or segmentation-fault
  char *p,*q;
  { FILE *stream = fopen("test.vilde","r");
    fseek(stream,0,SEEK_END);
    long length = ftell(stream);
    p = calloc(1,/* \n*/1 + length + /* \n*/1 + /* \0*/1);
    *p = '\n'; /* ( ? \n ? */
    fseek(stream,0,SEEK_SET);
    fread(p+1,1,length,stream);
    p[1 + length] = '\n'; /* ) ? \n ? */
    p[2 + length] = '\0';
    q = p + 2 + length;
    fclose(stream); }
  puts("~~~TO");
  Stoken *token;
  token = tokenize(p,q,tok_dclr,len_tok_dclr);
  puts("~~~RE");
  token = randlrun(token);
  { //retokenize remaining affixes
    Stoken *tmp = token;
    while(token){
      if(tt.NUMBER
	 & saffix)
	tta.retokenizer(token);
      token = token -> nxt;
    }
    token = tmp;
  }
  puts("~~~CU");
  while(token){
    if(tt.NUMBER
       & scalan)
      {
	ttc.cutenizer(token);
      }
    token = token->nxt;
  }
  return 0;
}
