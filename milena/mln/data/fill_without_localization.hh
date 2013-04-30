// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DATA_FILL_WITHOUT_LOCALIZATION_HH
# define MLN_DATA_FILL_WITHOUT_LOCALIZATION_HH

/// \file
///
/// Fill the contents of an image with another one without taking
/// into account the localization of sites.

# include <mln/core/concept/image.hh>
# include <mln/core/box_runstart_piter.hh>

# include <mln/border/get.hh>

namespace mln
{

  namespace data
  {

    /*!\brief Fill the contents of image \p ima with the image \p
     * data without taking into account the localization of sites.
     *
     * \param[in] ima data The image in which values are
     * assigned.
     * \param[in,out] The image providing pixels values.
     *
     * \ingroup mlndata
     */
    template <typename I, typename J>
    void fill_without_localization(Image<I>& ima, const Image<J>& data);



# ifndef MLN_INCLUDE_ONLY


    // Tests

    namespace internal
    {


	template <typename I, typename J>
	inline
	void fill_without_localization_tests(Image<I>& ima,
					     const Image<J>& data)
	{
	  mlc_converts_to(mln_value(I), mln_value(J))::check();

	  (void) ima;
	  (void) data;
	}

    } // end of namespace mln::data::internal



    // Implementations


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename J>
	inline
	void fill_without_localization(Image<I>& ima_,
				       const Image<J>& data_)
	{
	  mln_trace("data::impl::generic::fill_without_localization");

	  internal::fill_without_localization_tests(ima_, data_);

	  I& ima = exact(ima_);
	  const J& data = exact(data_);

	  mln_fwd_piter(I) pi(ima.domain());
	  pi.start();
	  mln_fwd_piter(J) po(data.domain());
	  po.start();
	  while (pi.is_valid() && po.is_valid())
	  {
	    ima(pi) = data(po);
	    pi.next();
	    po.next();
	  }

	}


      } // end of namespace mln::data::impl::generic



      /*! \brief Fill data to an image without using localization.
	Performs a one-block memory copy.

	\p ima and \p data must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same border size.
	- Same domain size.
	- Same value type.

       */
      template <typename I, typename J>
      inline
      void fill_without_localization_fastest(Image<I>& ima_,
					     const Image<J>& data_)
      {
	mln_trace("data::impl::fill_without_localization_fastest");

	internal::fill_without_localization_tests(ima_, data_);

	I& ima = exact(ima_);
	const J& data = exact(data_);

	typedef mln_value(I) V;
	memcpy(ima.buffer(), data.buffer(), ima.nelements() * sizeof(V));

      }



      /*! \brief Fill data to an image without using localization.
	Performs a line-per-line memory copy.

	\p ima and \p data must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same domain size.
	- Same value type.

       */
      template <typename I, typename J>
      inline
      void fill_without_localization_lines(Image<I>& ima_,
					   const Image<J>& data_)
      {
	mln_trace("data::impl::fill_without_localization_fastest");

	internal::fill_without_localization_tests(ima_, data_);

	I& ima = exact(ima_);
	const J& data = exact(data_);

	box_runstart_piter<mln_site(I)> pi(ima.domain());
	box_runstart_piter<mln_site(J)> po(data.domain());

	typedef mln_value(I) V;

	for_all_2(pi, po)
	  memcpy(&ima(pi), &data(po), ima.ncols() * sizeof(V));

      }



      /*! \brief Fill data to an image without using localization.
	Performs a point-wise copy.

	\p ima and \p data must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same domain size.

       */
      template <typename I, typename J>
      inline
      void fill_without_localization_fast(Image<I>& ima_,
					  const Image<J>& data_)
      {
	mln_trace("data::impl::fill_without_localization_fast");

	internal::fill_without_localization_tests(ima_, data_);

	I& ima = exact(ima_);
	const J& data = exact(data_);

        mln_pixter(I) pi(ima);
        mln_pixter(const J) po(data);

        for_all_2(pi, po)
          pi.val() = po.val();

      }



    } // end of namespace mln::data::impl



    // Dispatch

    namespace internal
    {

      template <typename I, typename J>
      inline
      void fill_without_localization_dispatch(
	mln::trait::image::value_access::direct,
	mln::trait::image::value_access::direct,
	mln::trait::image::ext_domain::some,
	mln::trait::image::ext_domain::some,
	I& ima,
	const J& data)
      {
        if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
        {
          if (border::get(ima) == border::get(data)
	      && ima.domain() == data.domain())
            impl::fill_without_localization_fastest(ima, data);
          else
            impl::fill_without_localization_lines(ima, data);
        }
        else
          impl::fill_without_localization_fast(ima, data);

      }


      template <typename I, typename J>
      inline
      void fill_without_localization_dispatch(
	mln::trait::image::value_access::any,
	mln::trait::image::value_access::any,
	mln::trait::image::ext_domain::any,
	mln::trait::image::ext_domain::any,
	I& ima,
	const J& data)
      {
	impl::generic::fill_without_localization(ima, data);
      }


      template <typename I, typename J>
      inline
      void fill_without_localization_dispatch(
	mln::trait::image::value_storage::any,
	mln::trait::image::value_storage::any,
	Image<I>& ima,
	const Image<J>& data)
      {
	impl::generic::fill_without_localization(ima, data);
      }


      template <typename I, typename J>
      inline
      void fill_without_localization_dispatch(
	mln::trait::image::value_storage::one_block,
	mln::trait::image::value_storage::one_block,
	Image<I>& ima_,
	const Image<J>& data_)
      {
        I& ima  = exact(ima_);
        const J& data = exact(data_);


        /// Check basic properties
        if (mlc_is(mln_trait_image_value_alignment(I),
                   trait::image::value_alignment::with_grid)::value &&
            mlc_is(mln_trait_image_value_alignment(J),
                   trait::image::value_alignment::with_grid)::value)
          {
            fill_without_localization_dispatch(
	      mln_trait_image_value_access(I)(),
	      mln_trait_image_value_access(J)(),
	      mln_trait_image_ext_domain(I)(),
	      mln_trait_image_ext_domain(J)(),
	      ima, data);
          }
        else
          impl::generic::fill_without_localization(ima, data);
      }


      template <typename I, typename J>
      inline
      void fill_without_localization_dispatch(Image<I>& ima,
					      const Image<J>& data)
      {
        fill_without_localization_dispatch(mln_trait_image_value_storage(I)(),
					    mln_trait_image_value_storage(J)(),
					    ima, data);
      }

    } // end of namespace mln::data::internal




    // Facade

    template <typename I, typename J>
    inline
    void fill_without_localization(Image<I>& ima, const Image<J>& data)
    {
      mln_trace("data::fill_without_localization");

      internal::fill_without_localization_tests(ima, data);

      internal::fill_without_localization_dispatch(ima, data);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln



#endif // ! MLN_DATA_FILL_WITHOUT_LOCALIZATION_HH
