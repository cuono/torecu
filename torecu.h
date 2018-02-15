#ifndef TORECU_H
#define TORECU_H

#include "miscellaneous.h"
#include "token.h"
#include "tokenizer.h"
#include "retokenizer.h"
#include "cutenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TORECU(foo)\
  CU(foo);\
  RE(foo);\
  TO(foo);

#endif
