#ifndef OLENA_FANCY_IOTA_HH
# define OLENA_FANCY_IOTA_HH

# include <oln/core/2d/image2d.hh>
# include <oln/core/1d/image1d.hh>

namespace oln {

  namespace fancy {


    namespace impl {
      template <typename T> 
      void iota(image1d<T>& inout); // FIXME: abstract::image1d<I>...
      template <typename T> 
      void iota(image2d<T>& inout); // FIXME: abstract::image2d<I>...
    } // end of namespace impl



    // facade

    template <typename I>
    void iota(abstract::image<I>& inout)
    {
      impl::iota(inout.exact());
    }



    // impl

    namespace impl {

      // FIXME: must be generic, of course ! But for now, we have not yet
      // iterators and we just want to test.

      template <typename T>
      void iota(image2d<T>& inout)
      {
	unsigned counter = 0;
        // FIXME: lacks cleaning
	for (coord_t row = 0; row < inout.size().nrows(); ++row) 
	  for (coord_t col = 0; col < inout.size().ncols(); ++col)
	    inout[point2d(row,col)] = ++counter;
      }

      template <typename T>
      void iota(image1d<T>& inout)
      {
	unsigned counter = 0;
        // FIXME: lacks cleaning
	for (coord_t index = 0; index < inout.size().nindices(); ++index)
	  inout[point1d(index)] = ++counter;
      } 

    } // end of namespace impl


  } // end of namespace fancy


} // end of namespace oln


#endif // ! OLENA_FANCY_IOTA_HH
