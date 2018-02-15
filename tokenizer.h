#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "token.h"

/* tox and toy for X,Y in buffer 
   eg blank.c and newline.c to
   ++X and ++Y, X = 1 */
extern int tox;
extern int toy;

/* typedef */
typedef struct Stok_dclr             Stok_dclr;
typedef struct Stokenizer            Stokenizer;

/* TO interface 
   a TO is C-function named tokenizer
   a tokenizer operate on to->p and
   totoken yield malloc'd token (Stoken) prepended to to->token
           use { and } for several
	   use { and } for each totoken
   tomatch yield malloc'd match (   str)
   after totoken token is a local variable
   after tomatch match is a local variable 
   HINT; use { and } (scope) for sequence of totoken */
#define TO(foo) void TO##foo (Stokenizer *tokenizer)
#define to tokenizer /* abbreviation ie to->token to->p */
#define totoken(TTN) to_new_token(tokenizer); Stoken *token = tokenizer->token; token->type.NUMBER = (TTN);
#define tomatch char *match = to_get_match(tokenizer)
#define toprint { tomatch; printf("%s %20s\n",match,"addition"); }
#define toaffix(TYPE,ARITY,PRECEDENCE,RETOKENIZER)\
  { tta.type = TYPE; tta.arity = ARITY;\
  tta.precedence = PRECEDENCE;\
  tta.retokenizer = RETOKENIZER; }
#define torandl(RORL,RETOKENIZER)\
  { ttr.rorl = RORL;\
    ttr.retokenizer = RETOKENIZER; }

/* C-function declaration */
Stoken     *tokenize(char *p,char *q,Stok_dclr *tok_dclr,int len_tok_dclr);
void    to_new_token(Stokenizer *tokenizer);
char   *to_get_match(Stokenizer *tokenizer);

/* tokenize() require declaration of TO
   as Stok_dclr */
struct Stok_dclr
{
  /*
    regular expression
    regex_cmp() ;; miscellaneous.c
  */
  char *regex;
  /*
    1 if regex
    0 if not
    strcmp()
  */  
  int isreg;  
  /*<#!
  ' lower the level more the priority to
  ' engage comparison of this regex
  ' hint: "a-z-identifier" ;; constant string
  ' hint: "a-z+"           ;; variable string
  ' you'd want "a-z+" to compare unless
  ' "a-z+" occured as "a-z-identifier"
  ' therefore "a-z-identifier" on lower level than "a-z+"
  \*/
  int level;
  /*<#!
  ' on each level, higher the length more
  ' the priority to engage comparison
  ' of this regex
  ' hint: "foo"        ;; constant string
  ' hint: "foofoo"     ;; constant string
  ' you'd want "foofoo" to compare unless
  ' "foofoo" occured as "foo foo"
  ' you'd want "foo" to compare unless
  ' "foo" occured as "foofoo"
  \*/
  #define LONGEST ((unsigned)~0)
  unsigned length;
  /*
    a subprogram which is run upon
    successful comparison of regex
    to do remaining tokenization
    such as creating a Stoken and
    filling it in
  */
  void(*tokenizer)(Stokenizer *); // rename to subprogram?
};

/*
  passed to TO in TORECU/whatever_name.c
 */
struct Stokenizer
{
  /*
    singly linked list
    in reverse chronological order,
    ie, prepend do not append Stoken
    NOTE tokenizer.c to_new_token totoken */
  Stoken *token;
  /*<#!
  ' p address towhere regex compared (current point)
  ' q last address of total input + 1 (EOF)
  \*/
  char *p,*q;
  /*<#!
  ' strlen of string regex compared
  ' on (the identifier / the raw token)
  ' strncpy(identifier,p,length)
  \*/
  unsigned length;
  /*
    the token's declaration
  */
  Stok_dclr *tok_dclr;
  //Stok_dclr2 *tok_dclr2;
};

#endif
