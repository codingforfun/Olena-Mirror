#include <oln/io/write_image.hh>
#include <oln/io/read_image.hh>

#include <oln/core/abstract/piter.hh>

#include <ntg/all.hh>

#include "check.hh"
#include "data.hh"


template<typename E>
bool compare(const oln::abstract::image<E>& a,
	     const oln::abstract::image<E>& b)
{

  oln_type_of(E, fwd_piter) it(a.size());

  if (a.size().nrows() != b.size().nrows() ||
      a.size().ncols() != b.size().ncols())
    return false;

  for_all_p(it)
    {
      if (a[it] != b[it])
	return false;
    }

  return true;
}

template< typename T >
bool
loadsave(const std::string& name, const std::string& savename)
{
  bool fail = false;

  T im1, im2;

  im1 = oln::io::read(name);


//   oln::io::write(im1, savename);


//   im2 = oln::io::read(savename);
  im2 = oln::io::read(name);

  if (compare(im1, im2) == true)
    std::cout << "OK" << std::endl;
  else
    {
      std::cout << "FAILED" << std::endl;
      fail = true;
    }
  return fail;
}
