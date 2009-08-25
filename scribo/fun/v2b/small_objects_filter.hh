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

#ifndef SCRIBO_FUN_V2B_SMALL_OBJECTS_FILTER_HH
# define SCRIBO_FUN_V2B_SMALL_OBJECTS_FILTER_HH

/// \file
///
/// Remove small objects in a binary image.



# include <mln/core/concept/function.hh>

# include <mln/util/array.hh>

# include <mln/accu/math/count.hh>

# include <mln/labeling/compute.hh>

# include <scribo/core/object_image.hh>


namespace scribo
{

  namespace fun
  {

    namespace v2b
    {

      using namespace mln;


      /// Filter Functor.
      /// Return false for all objects which are too small.
      template <typename L>
      struct small_objects_filter
	: Function_v2b< small_objects_filter<L> >
      {
	typedef accu::math::count<mln_psite(L)> card_t;

	/// Constructor
	///
	/// \param[in] objects Component bounding boxes.
	/// \param[in] min_size Minimum component size.
	//
	small_objects_filter(const object_image(L)& objects,
			     unsigned min_size);


	/// Check if the component is large enough.
	///
	/// \param l A label.
	///
	/// \return false if the component area is strictly inferion to
	/// \p min_size_.
	//
	bool operator()(const mln_value(L)& l) const;

	/// The component bounding boxes.
	mln::util::array<mln_result(card_t)> card_;

	/// The minimum area.
	unsigned min_size_;
      };




# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      inline
      small_objects_filter<L>::small_objects_filter(
	const object_image(L)& objects,
	unsigned min_size)
      {
	card_ = labeling::compute(card_t(), objects, objects.nlabels());
	min_size_ = min_size;
      }



      template <typename L>
      inline
      bool
      small_objects_filter<L>::operator()(const mln_value(L)& l) const
      {
	if (l == literal::zero)
	  return true;
	return card_[l] >= min_size_;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::fun::v2b

  } // end of namespace scribo::fun

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_SMALL_OBJECTS_HH
