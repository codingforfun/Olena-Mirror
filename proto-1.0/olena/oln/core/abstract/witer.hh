// Copyright (C) 2001, 2003, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_WITER_HH
# define OLENA_CORE_ABSTRACT_WITER_HH

# include <mlc/any.hh>
# include <mlc/types.hh>

# include <oln/core/properties.hh>

# include <string>

namespace oln {

  // fwd decl
  namespace abstract {
    template <typename E> struct witer;
  }

  // category
  template <typename E>
  struct set_category< abstract::witer<E> > { typedef category::witer ret; };

  /// properties of any type in category::witer
  //
  template <typename type>
  struct props_of < category::witer, type >
  {
    typedef mlc::true_type user_defined_;

    mlc_decl_prop(category::witer, se_type);

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( category::witer, "
	   << typeid(type).name() << ") = {"
           << "  se_type = " << typeid(se_type).name()
           << "  }" << std::endl;
    }
  };

  mlc_register_prop(category::witer, se_type); 

  namespace abstract {

    template <typename E>
    struct witer : public mlc::any__best_speed<E>
    {

      typedef witer<E> self_type;

      typedef oln_type_of(E, se)  se_type;
      typedef oln_type_of(se_type, dpoint)  dpoint_type;

      void start()
      {
        this->exact().impl_start();
      }

      void next()
      {
        precondition(this->is_valid());
        this->exact().impl_next();
      }

      bool is_valid() const
      {
        return this->exact().impl_is_valid();
      }

      operator dpoint_type() const
      {
        precondition(this->is_valid());
        return this->se_[pos_];
      }

      void invalidate()
      {
        this->exact().impl_invalidate();
        postcondition(! this->is_valid());
      }
 
    protected:

      void impl_start()
      {
        pos_ = 0;
      }

      void impl_next()
      {
        ++pos_;
      }

      bool impl_is_valid() const
      {
        return pos_ != se_.card();
      }

      void impl_invalidate()
      {
        pos_ = se_.card();
      }

      witer(const se_type& se) 
        : se_(se), pos_(0) 
      {}

      const se_type& se_;
      unsigned pos_;
    };

  } // abstract

} // oln

#endif // OLENA_CORE_ABSTRACT_WITER_HH
