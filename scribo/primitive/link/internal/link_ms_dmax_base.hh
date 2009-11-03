// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_MS_DMAX_HH_
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_MS_DMAX_HH_

/// \file
///
/// Base class for link functors using mass centers and a given max
/// distance.


# include <mln/accu/center.hh>
# include <mln/labeling/compute.hh>
# include <mln/math/abs.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_image.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/link/internal/find_link.hh>
# include <scribo/primitive/link/internal/link_functor_base.hh>

# include <scribo/primitive/link/compute.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	/// \brief Base class for link functors using mass centers and
	/// a given max distance.
	//
	template <typename L, typename E>
	class link_ms_dmax_base
	  : public link_functor_base<L, E>
	{
	  typedef link_functor_base<L,E> super_;
	  typedef mln_result(accu::center<mln_psite(L)>) ms_t;

	public:

	  typedef mln_site(L) P;


	  link_ms_dmax_base(const object_image(L)& objects,
			    unsigned neighb_max_distance);



	  bool verify_link_criterion_(unsigned current_object,
				  const P& start_point, const P& p) const;

	  mln_site(L) start_point_(unsigned current_object,
				   unsigned anchor);

	  void start_processing_object_(unsigned current_object);

	private:
	  mln::util::array<ms_t> mass_centers_;
	  float dmax_;
	  float neighb_max_distance_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_ms_dmax_base<L, E>::link_ms_dmax_base(
	  const object_image(L)& objects,
	  unsigned neighb_max_distance)

	  : super_(objects),
	    dmax_(0),
	    neighb_max_distance_(neighb_max_distance)
	{

	  mass_centers_ = labeling::compute(accu::meta::center(),
					    objects, objects.nlabels());
	}

	template <typename L, typename E>
	inline
	bool
	link_ms_dmax_base<L, E>::verify_link_criterion_(unsigned current_object,
							const P& start_point,
							const P& p) const
	{
	  (void) current_object;

	  float dist = math::abs(p.col() - start_point.col());
	  return dist <= dmax_; // Not too far
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_ms_dmax_base<L, E>::start_point_(unsigned current_object,
					      unsigned anchor)
	{
	  (void) anchor;
	  return mass_centers_(current_object);
	}


	template <typename L, typename E>
	inline
	void
	link_ms_dmax_base<L, E>::start_processing_object_(
	  unsigned current_object)
	{
	  float
	    midcol = (this->objects_.bbox(current_object).pmax().col()
		      - this->objects_.bbox(current_object).pmin().col()) / 2;
	  dmax_ = midcol + neighb_max_distance_;
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_MS_DMAX_HH_
