#ifndef TOKEN_H
#define TOKEN_H

typedef struct Stoken                Stoken;    /* reserved */

/*-----------------------------
  CONFIGURATION TO PROGRAMMER
------------------------------*/

//-------add/remove typedef for token-type struct (definition is below)
typedef struct Zrandl                Zrandl; /* reserved */
typedef struct Zaffix                Zaffix; /* reserved */
typedef struct Zcalan                Zcalan; /* reserved? */

//-------add/remove Stoken member
#define Stoken__add \
  int x/*row?*/; /* reserved? */ \
  int y/*col?*/; /* reserved? */

//-------add/remove token-type
#define Stoken_dot_type__add \
  Zaffix    affix;  /* reserved */ \
  Zrandl    randl;  /* reserved */ \
  Zcalan    calan;  /* reserved? */ \
  Zsymbol   symbol; \
  Znumber   number; \
  Zstring   string; 

//-------add/remove token-type-number symbol
#define snil           0x00000000 /* reserved */
#define seol           0x00000001 /* reserved */ //end of list
#define srandl         0x00000002 /* reserved */
#define saffix         0x00000004 /* reserved */
#define scalan         0x00000008 /* reserved? */
#define slist          0x00000010 /* reserved? */
#define ssymbol        0x00000020 /* temporary */
#define snumber        0x00000040 /* temporary */ /* 64 */
#define sendln         0x00000080 /* temporary? */
#define sxxx000        0x00000100
#define sxxx001        0x00000200
#define sxxx002        0x00000400
#define sxxx003        0x00000800
#define sstring        0x00001000
#define stype          0x00002000 /* BOR with type to denote itas type? 
				     eg as stype | sstring 
				     or as stype | snumber | sstring */

//-------add/remove definition of TT
typedef struct Zsymbol Zsymbol;
struct Zsymbol { int NUMBER; char  s; };
typedef struct Znumber Znumber;
struct Znumber { int NUMBER;  int  n; };
typedef struct Zstring Zstring;
struct Zstring { int NUMBER; char *s; };

//-------add/remove definition of TT (RESERVED)
struct Zaffix{ // TT is RESERVED
  int NUMBER;  // HIDDEN
#define NOFIX  0x0000
#define PREFIX 0x0001
#define INFIX  0x0002
#define SUFFIX 0x0004
  int type;
  int arity;
  int precedence;
  //void(*retokenizer)(Stoken *);
  Stoken *(*retokenizer)(Stoken *);
};
struct Zrandl{ // TT is RESERVED ?
  int NUMBER;  // HIDDEN
#define randl_reservedR   -2 //NOTE; R is even
#define randl_reservedL   -1 //NOTE; L is  odd
#define randl_parenthesisR 0
#define randl_parenthesisL 1
#define randl_curlyR       2
#define randl_curlyL       3
#define randl_bracketR     4
#define randl_bracketL     5
#define randl_indentR      6
#define randl_indentL      7
  int rorl; /* use randl_xxxR or randl_xxxL for specific
	       use rorl % 2 == 0 for R
	       use rorl % 2 != 0 for L 
	       rorl is R then rorl + 1 is randl_xxxL */
  //void(*retokenizer)(Stoken *);
  Stoken *(*retokenizer)(Stoken *);
};
struct Zcalan{   // TT is RESERVED
  int NUMBER;    // HIDDEN
  int argc;      // count of arguments
  Stoken **argv; // arguments
  int retu;      // ttn is returned by cutenizer
  Stoken *(*cutenizer)(Stoken *);
};

/* C-function declaration */
void affix2calan_vc(Stoken *token, Stoken *l);
Stoken *unlink_argl(Stoken *token);

/* macro for access to ttn */
#define tt        (token->type)
#define TT(T)     ((T)->type)
#define tta       (token->type.affix)
#define TTA(T)    ((T)->type.affix)
#define ttr       (token->type.randl)
#define TTR(T)    ((T)->type.randl)
#define ttc       (token->type.calan)
#define TTC(T)    ((T)->type.calan)
//#define ttcv(I)   (token->type.calan).argv[I]
//#define TTCV(T,I) ((T)->type.calan).argv[I]

#define typeck(...)
#define ttnchk(TOKEN,...) check_types(TOKEN, (int64_t[]){__VA_ARGS__}, sizeof((int64_t[]){__VA_ARGS__}) / sizeof(int64_t))

/* linked-list stuff for use on Stoken */
#define unlink_lr(l,r)     (l)->prv->nxt = (r)->nxt, (r)->nxt->prv = (l)->prv, (l)->prv = NULL, (r)->nxt = NULL
#define unlink(TOKEN)      unlink_lr(TOKEN,TOKEN)
#define linkin(src,l,r)    (l) -> nxt = (src), (r) -> prv = (src), (src) -> prv = (l), (src) -> nxt = (r)
#define linkin_lr(a,z,l,r) (l) -> nxt = (a), (r) -> prv = (z), (a) -> prv = (l), (z) -> nxt = (r)
#define linkout(src,l,r)   (src) -> prv = (l) -> prv,

/*
  passed to RE and CU in TORECU/whatever_name.c
 */
struct Stoken
{
  union Utoken_type
  {
    int NUMBER;
    Stoken_dot_type__add
  }type;
  Stoken__add
  struct Stoken *prv,*nxt;
};

#endif
