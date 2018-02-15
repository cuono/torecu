

TORECU for TO and RE and CU
TO is text to token or tokenization
RE is token to token or retokenization aka in matter of tokenization like mail
CU is cutenization aka execution when tokens is run like program
TORECU can interpret no particular language
as long as syntax for lanaguage is limited
to affixes like suffix and prefix and infix
and randls like ')' and '(' is RandL aka pair or '}' and '{' is randl
written in C
can embed in program for scripting or similar

algorithm use token
token got ttn for token-type-number denoting type for uniqueness
look for ttn in token.h
eg of ttn is snumber for number
eg of ttn is sstring for string etc
token is object which can be different in ttn
some ttn is implemented for algorithm aka reserved
rest ttn is written by you aka unimplemented
each ttn is token at root thus
we access to ttn by macro
tt        for token->type eg tt.number = 10
TT(T)     for (T)->type   eg TT(token1).number
tta       for token->type.affix
TTA(T)    for (T)->type.affix
ttr       for token->type.randl
TTR(T)    for (T)->type.randl
ttc       for token->type.calan
TTC(T)    for (T)->type.calan










token and ttn
tokenization
  tokenize
  TO
retokenization
  affix
  randl
  calan
  un_slash_ambiguity
  LorR
  randlrun
  RE
cutenization
  CU
