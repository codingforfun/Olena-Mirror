// Copyright (C) 2001, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_HH

# include <mlc/type.hh>
# include <oln/core/coord.hh>

namespace oln {
  namespace abstract {

    // fwd_decl
    template <class Exact>
    class image;

  } // end of namespace abstract

  template<class Exact>
  struct image_id;

  template<class Ima>
  struct image_traits;

  template <class Exact>
  struct image_traits<abstract::image<Exact> >
  {
    typedef Exact exact_type;
  };

  namespace abstract {

    template <class Exact>
    class image : public mlc::any<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type;
      typedef typename image_traits<Exact>::dpoint_type dpoint_type;
      typedef typename image_traits<Exact>::iter_type iter_type;
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<Exact>::value_type value_type;
      typedef typename image_traits<Exact>::size_type size_type;
      typedef typename image_traits<Exact>::impl_type impl_type;
      typedef image<Exact> self_type;
      typedef Exact exact_type;

      enum { dim = image_traits<Exact>::dim };

      const value_type& operator[](const point_type& p) const
      {
  	return to_exact(this)->at(p);
      }

      value_type& operator[](const point_type& p)
      {
	return to_exact(this)->at(p);
      }

      exact_type clone() const
      {
	return to_exact(this)->clone_();
      }

      bool hold(const point_type& p) const
      {
	assertion(has_impl_());
	return to_exact(this)->impl()->hold(p);
      }

      const size_type& size() const
      {
	assertion(has_impl_());
	return to_exact(this)->impl()->size();
      }

      coord border() const
      {
	return size().border();
      }

      size_t npoints() const
      {
	return to_exact(this)->npoints_();
      }

      exact_type& operator=(self_type rhs)
      {
	return to_exact(this)->assign(to_exact(rhs));
      }

      static std::string name()
      {
	return
	  std::string("abstract::image<")
	  + Exact::name() + ">";
      }

      // borders

      void border_set_width(coord new_border, bool copy_border = false) const
      {
	precondition(new_border >= 0);
	precondition(has_impl_());
	if (border() == new_border)
	  return;			// Nothing to do.
	
	const_cast<impl_type *>(to_exact(this)->impl())->border_reallocate_and_copy(new_border, copy_border);
      }

      void border_adapt_width(coord min_border, bool copy_border =
			      false) const
      {
	precondition(min_border >= 0);
	if (border() >= min_border)
	  return;			// Don't shrink.
	
	to_exact(this)->border_set_width(min_border, copy_border);
      }

      void border_adapt_copy(coord min_border) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(to_exact(this)->impl())->border_replicate();
      }


      void border_adapt_mirror(coord min_border) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(to_exact(this)->impl())->border_mirror();
      }

      void border_adapt_assign(coord min_border, value_type val) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(to_exact(this)->impl())->border_assign(val);
      }
    protected:
      image() {}
      //image(self_type& rhs) {}

      bool has_impl_() const
      {
	return to_exact(this)->impl() != 0;
      }
    };

  } // end of namespace abstract


  


} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_HH
