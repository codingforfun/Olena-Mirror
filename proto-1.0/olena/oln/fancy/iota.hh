#ifndef OLENA_FANCY_IOTA_HH
# define OLENA_FANCY_IOTA_HH

# include <oln/core/2d/image2d.hh>

namespace oln {

  namespace fancy {


    namespace impl {
      template <typename T> void iota(image2d<T>& inout); // FIXME: abstract::image2d<I>...
    } // end of namespace impl



    // facade

    template <typename I>
    void iota(abstract::image<I>& inout)
    {
      impl::iota(inout.exact());
    }



    // impl

    namespace impl {

      template <typename T>
      void iota(image2d<T>& inout)
      {
	unsigned counter = 0;
	for (coord_t row = 0; row < inout.size().nrows(); ++row) // FIXME: lacks cleaning
	  for (coord_t col = 0; col < inout.size().ncols(); ++col)
	    inout[point2d(row,col)] = ++counter;
      }

    } // end of namespace impl


  } // end of namespace fancy


} // end of namespace oln


#endif // ! OLENA_FANCY_IOTA_HH
