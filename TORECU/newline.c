#include "newline.h"
#include "xy.h"

TO(newline)
{
  totoken(sendln);
  X = xy_start_x; Y += 1;
}
