#include "conv.hh"

/* The extra indirection to the _STR and _CONC macros is required so that
   if the arguments to STR() (or CONC()) are themselves macros, they will
  ls
 be expanded before being quoted.   */
#ifndef STR
# define _STR(arg)     #arg
# define STR(arg)      _STR(arg)
#endif

#ifndef CONC
# define _CONC(a, b)   a##b
# define CONC(a, b)    _CONC(a, b)
#endif

#include STR(oln/convert/CONC(SRC,DEST).hh)

bool check()
{
  
  CONC(SRC,_8) dep, arr;
  CONC(DEST,_8) tmp;
 
  for(int i = 0; i < 256; i += 3)
    for(int j = 0; j < 256; j += 3)
      for(int k = 0; k < 256; k += 3)
	{
	  CONC(SRC,_8) orig(i, j, k);
	  CONC(DEST,_8) tmp = convert::CONC(CONC(SRC,_to_),DEST)<8, 8>(orig);
	  CONC(SRC,_8) res = convert::CONC(CONC(DEST,_to_),SRC)<8, 8>(tmp);
 
	  if(dist<CONC(SRC,_8)>(orig, res) > EM)
            {
	      return true;
	    }
	}
  return false;
}
