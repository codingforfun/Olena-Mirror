// Copyright (C) 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMPL_IMAGE_IMPL_HH
# define OLENA_CORE_IMPL_IMAGE_IMPL_HH

# include <oln/core/internal/fake.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <iostream>
# include <mlc/type.hh>


namespace oln {

  // FIXME: deplacer
  template<class Ima>
  struct image_traits;

  namespace impl {

    template<class ExactI, class Exact>
    class image_impl : public mlc::any<Exact>
    {
    public:
      typedef typename image_traits<ExactI>::point_type point_type;
      typedef typename image_traits<ExactI>::iter_type iter_type;
      typedef typename image_traits<ExactI>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<ExactI>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<ExactI>::value_type value_type;
      typedef typename image_traits<ExactI>::size_type size_type;

      typedef Exact exact_type;

      image_impl(): refcount_(0) {}

      void ref() const
      {
	++refcount_;
      }

      void unref() const
      {
	invariant(refcount_ > 0);
	--refcount_;
	if (refcount_ == 0)
	  delete this;
      }

      const value_type& at(const point_type& p) const
      {
	return to_exact(this)->at_(p);
      }

      value_type& at(const point_type& p) 
      {
	return to_exact(this)->at_(p);
      }

      bool hold(const point_type& p) const
      {
	return to_exact(this)->hold_(p);
      }

      bool hold_large(const point_type& p) const
      {
	return to_exact(this)->hold_large_(p);
      }

      void precondition_hold_large(const point_type& p) const
      {
# ifndef NDEBUG
	if (! hold_large(p))
	  std::cerr << "image overflow at (" << p << ")" << std::endl;
# else
	(void) p;
# endif
      }

      exact_type clone() const
      {
	return to_exact(this)->clone_();
      }

    private:
      mutable unsigned refcount_;

    };


  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_IMPL_HH
