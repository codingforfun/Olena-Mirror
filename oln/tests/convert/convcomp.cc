#include "convcomp.hh"

/* The extra indirection to the _STR and _CONC macros is required so that
   if the arguments to STR() (or CONC()) are themselves macros, they will
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
  image2d< CONC(SRC, _8) > orig(rdata("lena.ppm")), res;
  image2d< CONC(DEST, _8) > tmp;

  tmp  = convert::apply(convert::CONC(CONC(SRC,_to_),DEST)(), orig);
  res  = convert::apply(convert::CONC(CONC(DEST,_to_),SRC)(), tmp);

  utils::f_moments<sfloat> fm;
  compare_images(fm, f_l2_vec3<CONC(SRC,_8)>(), orig, res);

  float m = fm.mean();
  float v = fm.variance();

  std::cout << "mean     = " << m << std::endl;
  std::cout << "variance = " << v << std::endl;

  return m >= EM * 1.1 || v >= EV * 1.1;
}
