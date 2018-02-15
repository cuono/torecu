#ifndef ASSOCIATE_H
#define ASSOCIATE_H
#include "../torecu.h"
/*
  if some TORECU/whatever_name.c depends
  on some other TORECU/whatever_name.c
  eg this TORECU/associate.c
  it can just include TORECU/associate.h
 */

CU(associate);
RE(associate);
TO(associate);

typedef struct associate_t associate_t;
struct associate_t
{
  Stoken value;
};

#define associate_last_index ('z' - 'a')

Stoken *syono(Stoken *token);

#endif
