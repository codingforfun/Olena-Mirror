#ifndef OLENA_FANCY_IOTA_HH
# define OLENA_FANCY_IOTA_HH

# include <oln/core/macros.hh>
# include <oln/core/abstract/image.hh>

namespace oln {

  namespace fancy {


    // facade == impl

    template <typename I>
    void iota(abstract::image<I>& inout)
    {
      unsigned counter = 0;
      oln_piter_type(I) p(inout.size());
      for_all(p)
	inout[p] = ++counter;
    }


  } // end of namespace oln::fancy

} // end of namespace oln


#endif // ! OLENA_FANCY_IOTA_HH
