#include <oln/basics2d.hh>
#include <oln/io/pnm.hh>
#include <ntg/all.hh>

#include "check.hh"
#include "data.hh"

// note: don't use using namespace directives, they may hide wrong code

bool check()
{
  oln::image2d<ntg::range<oln::int_u16, ntg::bounded_u<0, 255> > > im1;
  oln::image2d<unsigned short> im2(oln::io::load(rdata("lena")));

  oln::image2d<bool> im3(oln::io::load(rdata("lena")));
  oln::image2d<ntg::bin> im4(oln::io::load(rdata("lena")));

  im1 = oln::io::load(rdata("lena"));
  im2 = oln::io::load(rdata("lena"));

  oln::io::save(im1, "lena1");
  oln::io::save(im2, "lena2");
  oln::io::save(im3, "lena3");
  oln::io::save(im4, "lena4");

  return false;
}
