#include "wav.h"

wave_mixer_t mixer={
  .wave={.spec={.rate=44100,
		.depth=16,
		.channels=1}},
  .spec={.rate=44100,
	 .depth=16,
	 .channels=1},
  .allocated = 0,
  .offset = 0
};

CU(wav)
{
  static Stoken RETU;
  TT(&RETU).NUMBER = ssymbol|stype;
  return &RETU;
}
RE(wav)
{
  reaffix2calan;
  recalan(ssymbol|stype,CUwav);
}
TO(wav)
{
  totoken(saffix);
  toaffix(PREFIX,1,0,REwav);
}
