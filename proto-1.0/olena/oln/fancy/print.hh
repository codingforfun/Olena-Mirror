#ifndef OLENA_FANCY_PRINT_HH
# define OLENA_FANCY_PRINT_HH


# include <iostream>
# include <string>


# include <ntg/real/int_u8.hh> // FIXME: no coupling like that!

# include <oln/core/1d/image1d.hh>
# include <oln/core/2d/image2d.hh>

namespace oln {

  namespace fancy {



    // fwd decls

    namespace impl {

      template <typename T>
      void print(const image1d<T>& input, std::ostream& ostr);
      template <typename T>
      void print(const image2d<T>& input, std::ostream& ostr);
      // FIXME: it should be abstract::image2d<I>...

    } // end of namespace impl



    // facades

    template <typename I>
    void print(const abstract::image<I>& input,
	       std::ostream& ostr = std::cout)
    {
      impl::print(input.exact(), ostr);
    }

    template <typename I>
    void println(const abstract::image<I>& input,
		 std::ostream& ostr = std::cout)
    {
      print(input, ostr);
      ostr << std::endl;
    }


    // impls

    namespace impl {

      namespace internal {

	// FIXME: awful

	template <typename T>
	struct pp_type { typedef T ret; };

	template <typename T>
	const typename pp_type<T>::ret pp(const T& t) { return t; }

	template <> struct pp_type <  ntg::int_u8 > { typedef unsigned int ret; };
	template <> struct pp_type <unsigned char > { typedef unsigned int ret; };
	template <> struct pp_type <  signed char > { typedef   signed int ret; };
	template <> struct pp_type <         char*> { typedef  std::string ret; };

      }

      template <typename T>
      void print(const image2d<T>& input, std::ostream& ostr)
      {
	// FIXME: lacks cleaning
	for (coord_t row = 0; row < input.size().nrows(); ++row)
	  {
	    for (coord_t col = 0; col < input.size().ncols(); ++col)
	      ostr << internal::pp<T>(input[point2d(row,col)]) << ' ';
	    ostr << std::endl;
	  }
      }

      template <typename T>
      void print(const image1d<T>& input, std::ostream& ostr)
      {
	// FIXME: lacks cleaning
	for (coord_t index = 0; index < input.size().nindices(); ++index)
	  {
	    ostr << internal::pp<T>(input[point1d(index)]) << ' ';
	  }
	ostr << std::endl;
      } 

    } // end of namespace impl


  } // end of namespace fancy


} // end of namespace oln


#endif // ! OLENA_FANCY_PRINT_HH
