// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CONVOL_NAGAO_HXX
# define OLN_CONVOL_NAGAO_HXX
# include <oln/core/window2d.hh>

namespace oln {
  namespace convol {
    namespace internal {

      template<class I, class SE, unsigned Size, typename Sum>
      inline Sum
      mean_of_smaller_variance(const oln::abstract::image<I> &im,
			       const oln_point_type(I) &p,
			       const internal::se_array<Size, SE> &w)
      {
	ntg_compare_nb_comp(Sum, oln_value_type(I))::ensure();

	precondition(Size > 0);
	precondition(im.border() >= w.delta());

	typedef typename oln::utils::se_stat<Sum, ntg::float_s> se_stat_type;
	typedef typename se_stat_type::sum_type sum_type;
	typedef typename se_stat_type::variance_type variance_type;

	se_stat_type s(im, p, w[0]);
	variance_type min_variance = s.variance();
	sum_type mean = s.mean();
	for (unsigned i = 1; i < Size; ++i)
	  {
	    variance_type tmp = s.compute(im, p, w[i]).variance();
	    if (tmp < min_variance)
	      {
		min_variance = tmp;
		mean = s.mean();
	      }
	  }
	return mean;
      }

      /*! \brief build 9 windows corresponding to the standard
      ** 5x5 Nagao filter.
      */
      static
      se_array<9, window2d>
      mk_nagao_windows_5x5()
      {
	se_array<9, window2d> out;
	//center				//.....
	out[0].add(-1, -1).add(-1, 0).add(-1, 1)//.XXX.
	  .add(0, -1).add(0, 0).add(0, 1)	//.XCX.
	  .add(1, -1).add(1, 0).add(1, 1);	//.XXX.
						//.....

	// upper				//.XXX.
	out[1].add(-2, -1).add(-2, 0).add(-2, 1)//.XXX.
	  .add(-1, -1).add(-1, 0).add(-1, 1)	//..C..
	  .add(0, 0);				//.....
						//.....

	// right				//.....
	out[2].add(-1, 1).add(-1, 2)		//...XX
	  .add(0, 0).add(0, 1).add(0, 2)	//..CXX
	  .add(1, 1).add(1, 2);			//...XX
						//.....
	// bottom				//.....
	out[3].add(2, -1).add(2, 0).add(2, 1)	//.....
	  .add(1, -1).add(1, 0).add(1, 1)	//..C..
	  .add(0, 0);				//.XXX.
						//.XXX.

	// left					//.....
	out[4].add(-1, -1).add(-1, -2)		//XX...
	  .add(0, 0).add(0, -1).add(0, -2)	//XXC..
	  .add(1, -1).add(1, -2);		//XX...
						//.....

	// upper-left				//XX...
	out[5].add(-2, -2).add(-2, -1)		//XXX..
	  .add(-1, -2).add(-1, -1).add(-1, 0)	//.XC..
	  .add(0, -1).add(0, 0);		//.....
						//.....

	// upper-right				//...XX
	out[6].add(-2, 2).add(-2, 1)		//..XXX
	  .add(-1, 2).add(-1, 1).add(-1, 0)	//..CX.
	  .add(0, 1).add(0, 0);			//.....
						//.....

	// bottom-right				//.....
	out[7].add(2, 2).add(2, 1)		//.....
	  .add(1, 2).add(1, 1).add(1, 0)	//..CX.
	  .add(0, 1).add(0, 0);			//..XXX
						//...XX

	// bottom-left				//.....
	out[8].add(2, -2).add(2, -1)		//.....
	  .add(1, -2).add(1, -1).add(1, 0)	//.XC..
	  .add(0, -1).add(0, 0);		//XXX..
      						//XX...
	return out;
      }
    } // ! end namespace internal


    template <class I, unsigned Size, typename SE, typename Sum>
    inline oln_concrete_type(I)
    nagao_generalized(const oln::abstract::image<I>& in,
		      const internal::se_array<Size, SE> &sa)
    {
      ntg_compare_nb_comp(Sum, oln_value_type(I))::ensure();
      in.border_adapt_width(sa.delta());
      oln_concrete_type(I) out(in.size());
      oln_iter_type(I) it(out);
      for_all(it)
      out[it] = internal::mean_of_smaller_variance<I, SE, Size, Sum>
	(in, it, sa);
      return out;
    }

    /*! Standard Nagao filter 5x5.
    **
    ** Apply the 5*5 nagao filter on a non vectorial image.
    ** \code
    ** #include <oln/convol/nagao.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::int_u8> imc = oln::load(IMG_IN  "lena256.pgm");
    **   oln::save(oln::convol::nagao(imc),
    ** 	    IMG_OUT "oln_convol_nagao_256.pgm");
    ** }
    ** \endcode
    ** \image html lena256_pgm.png
    ** \image latex lena256_pgm.png
    ** =>
    ** \image html oln_convol_nagao_256.png
    ** \image latex oln_convol_nagao_256.png
    */
    template <class I>
    inline oln_concrete_type(I)
    nagao(const oln::abstract::non_vectorial_image_with_dim<2, I>& in)
    {
      return nagao_generalized<I, 9, window2d, ntg::float_s>
	(in, internal::mk_nagao_windows_5x5());
    }

    /*! Standard Nagao filter 5x5.
    **
    ** Apply the 5*5 nagao filter on a vectorial image.
    **
    ** \code
    ** #include <oln/convol/nagao.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **    oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **    oln::save(oln::convol::nagao(imc), IMG_OUT "oln_convol_nagao.ppm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_convol_nagao.png
    ** \image latex oln_convol_nagao.png
    */
    template <class I>
    inline oln_concrete_type(I)
    nagao(const oln::abstract::vectorial_image_with_dim<2, I>& in)
    {
      typedef ntg::rgb_8::float_vec_type float_vec_type;
      return nagao_generalized<I, 9, window2d, float_vec_type>
	(in, internal::mk_nagao_windows_5x5());
    }
  } //! namespace convol
} //! namespace oln

#endif
