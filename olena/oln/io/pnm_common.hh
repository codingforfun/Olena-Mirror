#ifndef OLENA_IO_PNM_COMMON_HH_
# define OLENA_IO_PNM_COMMON_HH_

# include <mlc/bool.hh>

# include <ntg/basics.hh>

# include <oln/core/coord.hh>
# include <oln/core/image.hh>

namespace oln {

  namespace io {

    namespace internal {

      enum pnm_type { PnmInvalid   = 0,
		      PnmBinary    = 1,
		      PnmInteger   = 2,
		      PnmVectorial = 3 };
      
      struct pnm2d_info
      {
	coord cols;
	coord rows;
	unsigned max_val;
      };

      /*------------------------.
      | get_pnm_type from image |
      `------------------------*/

      template <pnm_type V>
      struct get_it
      {
	static const pnm_type ret = V;
      };

      // Return the pnm type associated to an image type

      template <class I>
      struct get_pnm_type
      {
	typedef typename mlc::bool_switch_<
	  mlc::bool_case_<ntg_is_a(Value(I), ntg::integer)::ret,
			  get_it<PnmInteger>,

	  mlc::bool_case_<ntg_is_a(Value(I), ntg::binary)::ret, 
			  get_it<PnmBinary>,

	  mlc::bool_case_<ntg_is_a(Value(I), ntg::vectorial)::ret,
			  get_it<PnmVectorial>,

	  mlc::bool_case_<true,
			  get_it<PnmInvalid>
	> > > > >::ret_t tmp_type;

	static const pnm_type ret = tmp_type::ret;
      };

    } // end of internal

  } // end of io

} // end of oln

#endif // ! OLENA_IO_PNM_COMMON_HH_
