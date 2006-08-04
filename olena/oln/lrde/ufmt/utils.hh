// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LRDE_UFMT_UTILS_HH
# define OLENA_LRDE_UFMT_UTILS_HH

# include <ntg/real/int_u.hh>
# include <vector>





# define oln_neighborhood_type(I) \
typename oln::lrde::ufmt::internal::neighborhood<I>::ret



namespace oln
{


  // Fwd decls.
  struct neighborhood1d;
  struct neighborhood2d;
  struct neighborhood3d;
  template <class T, class E> class image1d;
  template <class T, class E> class image2d;
  template <class T, class E> class image3d;
  // end of fwd decls.



  namespace lrde
  {

    namespace ufmt
    {


      // Get the number of possible different unsigned integer values
      // of the input image data; for instance, when 'input' contains
      // int_u8 data, the result is 256.

      template <class I>
      size_t uint_nvalues(const abstract::image<I>& input)
      {
	typedef oln_value_type(I) T;
	typedef typename ntg_is_a(T, ntg::unsigned_integer)::ensure_type ensure_type;
	return size_t(ntg_max_val(T)) + 1;
      }

      template <class T>
      size_t uint_nvalues()
      {
	typedef typename ntg_is_a(T, ntg::unsigned_integer)::ensure_type ensure_type;
	return size_t(ntg_max_val(T)) + 1;
      }


      // Compute the input image histogram up to the Nth point of the
      // input image.

      template <class I>
      std::vector<size_t> histogram(const abstract::image<I>& input,
				    size_t N)
      {
	unsigned nvalues = uint_nvalues(input);
	std::vector<size_t> H(nvalues, 0);
	size_t count = 0;
	oln_iter_type(I) p(input);
	for_all(p)
	  {
	    ++H[input[p]];
	    if (++count == N)
	      return H;
	  }
	return H;
      }


      // Compute the input image histogram.

      template <class I>
      std::vector<size_t> histogram(const abstract::image<I>& input)
      {
	return histogram(input, input.npoints());
      }


      // FIXME: doc.

      template <class I>
      std::vector<oln_point_type(I)>
      histogram_reverse_sort_p(const abstract::image<I>& input,
			       size_t N,
			       std::vector<size_t>& H)
      {
	H = histogram(input, N);

	// preparing output data

	unsigned nvalues = uint_nvalues(input);
	std::vector<int> loc(nvalues);
	loc[nvalues - 1] = 0;
	for (int l = int(nvalues) - 2; l >= 0; --l)
	  loc[l] = loc[l+1] + H[l+1];

	std::vector<oln_point_type(I)> vec(N);

	// storing output data

	oln_iter_type(I) p(input);
	size_t count = 0;
	for_all(p)
	  {
	    vec[loc[input[p]]++] = p;
	    if (++count == N)
	      return vec;
	  }
	return vec;
      }



      template <class I>
      std::vector<oln_point_type(I)>
      histogram_reverse_sort_p(const abstract::image<I>& input,
			       std::vector<size_t>& H)
      {
	return histogram_reverse_sort_p(input, input.npoints(), H);
      }


      template <class I>
      std::vector<oln_point_type(I)>
      histogram_reverse_sort_p(const abstract::image<I>& input,
			       size_t N)
      {
	std::vector<size_t> H;
	return histogram_reverse_sort_p(input, N, H);
      }


      template <class I>
      std::vector<oln_point_type(I)>
      histogram_reverse_sort_p(const abstract::image<I>& input)
      {
	std::vector<size_t> H;
	return histogram_reverse_sort_p(input, input.npoints(), H);
      }


      // _i

      template <class I>
      std::vector<int>
      histogram_reverse_sort_i(const abstract::image<I>& input,
			       size_t N,
			       std::vector<size_t>& H)
      {
	H = histogram(input, N);

	// preparing output data

	unsigned nvalues = uint_nvalues(input);
	std::vector<int> loc(nvalues);
	loc[nvalues - 1] = 0;
	for (int l = int(nvalues) - 2; l >= 0; --l)
	  loc[l] = loc[l+1] + H[l+1];

	std::vector<int> vec(N);

	// storing output data

	typedef oln_value_type(I) value_t;
	typedef oln_point_type(I) point_t;

	I& input_ = const_cast<I&>(input.exact());
	value_t* orig = &(input_[point_t()]);
	oln_iter_type(I) p(input);
	size_t count = 0;
	for_all(p)
	  {
	    vec[loc[input[p]]++] = int(&(input_[p]) - orig);
	    if (++count == N)
	      return vec;
	  }

	return vec;
      }


      template <class I>
      std::vector<int>
      histogram_reverse_sort_i(const abstract::image<I>& input,
			       std::vector<size_t>& H)
      {
 	return histogram_reverse_sort_i(input, input.npoints(), H);
      }



      // Extra traits to get the classical neighborhood types from
      // classical image types.

      namespace internal {

	template <class I>
	struct neighborhood;

	template <class T>
	struct neighborhood < image1d<T> > { typedef neighborhood1d ret; };
	template <class T>
	struct neighborhood < image2d<T> > { typedef neighborhood2d ret; };
	template <class T>
	struct neighborhood < image3d<T> > { typedef neighborhood3d ret; };

      } // end of namespace oln::lrde::ufmt::internal



      template <class I>
      unsigned pre(const oln_neighborhood_type(I)& nbh,
		   std::vector<oln_dpoint_type(I)>& pre)
      {
	for (unsigned i = 0; i < nbh.card(); ++i)
	  {
	    unsigned d;
	    for (d = 0; d < I::dim; ++d)
	      if (nbh.dp(i).nth(d) < 0) {
		pre.push_back(nbh.dp(i));
		break;
	      }
	      else if (nbh.dp(i).nth(d) > 0)
		break;
	  }
	assert(pre.size() == (nbh.card() / 2));
	return pre.size();
      }


      template <class I>
      unsigned split(const oln_neighborhood_type(I)& nbh,
		     std::vector<oln_dpoint_type(I)>& pre,
		     std::vector<oln_dpoint_type(I)>& post)
      {
	for (unsigned i = 0; i < nbh.card(); ++i)
	  {
	    unsigned d;
	    bool is_post = true;
	    for (d = 0; d < I::dim; ++d)
	      if (nbh.dp(i).nth(d) < 0) {
		pre.push_back(nbh.dp(i));
		is_post = false;
		break;
	      }
	      else if (nbh.dp(i).nth(d) > 0)
		break;
	    if (is_post)
	      post.push_back(nbh.dp(i));
	  }
	if (not ((pre.size() + post.size()) == nbh.card())
	    or not (pre.size() == post.size()))
	  {
	    std::cerr << "pb in oln::lrde::ufmt::split (utils.hh) so abort!" << std::endl;
	    abort();
	  }
	assert((pre.size() + post.size()) == nbh.card());
	assert(pre.size() == post.size());
	return pre.size();
      }



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln




#endif // ! OLENA_LRDE_UFMT_UTILS_HH
