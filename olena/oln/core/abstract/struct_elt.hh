// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_STRUCT_ELT_HH
# define OLENA_CORE_STRUCT_ELT_HH

# include <mlc/type.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>

namespace oln {

  namespace abstract {

    template<class Exact>
    struct struct_elt; // fwd_decl

  } // end of abstract

  template<class Exact>
  struct struct_elt_traits;

  template<class Exact>
  struct struct_elt_traits<abstract::struct_elt<Exact> >
  {
    typedef abstract::struct_elt<Exact> abstract_type;
  };

  namespace abstract {

    template<class Exact>
    struct struct_elt : public mlc_hierarchy::any< Exact >
    {
      enum { dim = struct_elt_traits<Exact>::dim };

      typedef struct_elt<Exact> self_type;
      typedef typename struct_elt_traits<Exact>::abstract_type abstract_type;

      static std::string name()
      {
	return std::string("struct_elt<") + Exact::name() + ">";
      }

      typedef typename struct_elt_traits<Exact>::point_type point_type;
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;
      typedef Exact exact_type;

      bool has(const abstract::dpoint<dpoint_type>& dp) const
      {
	return this->exact().has_(dp.exact());
      }

      unsigned card() const
      {
	return this->exact().card_();
      }

      bool is_centered() const
      {
	return this->exact().is_centered_();
      }

      // FIXME: here for convenience (see morpho algorithms),
      // work with w_windows (associate a default weight set to 1)
      exact_type& add(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->exact().add_dp(dp);
      }

      dpoint_type dp(unsigned i) const
      {
	return this->exact().at(i);
      }

      bool operator==(const self_type& win) const
      {
	return this->exact().is_equal(win.exact());
      }

      coord delta() const
      {
	return this->exact().get_delta();
      }

      const dpoint_type operator[](unsigned i) const
      {
	return this->exact().at(i);
      }

      exact_type operator-() const
      {
	exact_type win(this->exact());
	win.sym();
	return win;
      }

    protected:

      void sym()
      {
	return this->exact().sym_();
      }

      struct_elt() {}
    };
  } // end of abstract

} // end of oln

#endif // ! OLENA_CORE_STRUCT_ELT_HH
