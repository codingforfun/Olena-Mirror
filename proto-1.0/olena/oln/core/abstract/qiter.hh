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

#ifndef OLENA_CORE_ABSTRACT_QITER_HH
# define OLENA_CORE_ABSTRACT_QITER_HH

# include <mlc/any.hh>
# include <oln/core/coord.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/abstract/window.hh>


# define oln_qit_type_of(QiterType, Alias) \
mlc_type_of(oln, oln::category::qiter, QiterType, Alias)


namespace oln {

  namespace category
  {
    struct qiter;
  }

  /// Default properties of any type in category::qiter.

  struct set_default_props < category::qiter >
  {
    typedef mlc::undefined_type window_type;
  };

  // props

  template <typename Q>
  struct get_props < category::qiter, Q >
  {
    typedef oln_qit_type_of(Q, window) window_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::qiter, " << mlc_to_string(Q) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t window_type = " << mlc_to_string(window_type) << std::endl
	   << "}" << std::endl;
    }
  };



  namespace abstract {

    template <typename E>
    struct qiter : public mlc::any__best_speed<E>
    {

      typedef qiter<E> self_type;

      typedef oln_qit_type_of(E, window)  window_type;
      typedef oln_wn_type_of(window_type, dpoint)  dpoint_type;

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
        return this->win_[pos_];
      }

      void invalidate()
      {
        this->exact().impl_invalidate();
        postcondition(! this->is_valid());
      }

      coord_t nth(unsigned i)
      {
	return this->win_[this->pos_].nth(i);
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
        return pos_ != win_.card();
      }

      void impl_invalidate()
      {
        pos_ = win_.card();
      }

      qiter(const window_type& se)
        : win_(se), pos_(0)
      {}

      const window_type& win_;
      unsigned pos_;
    };

  } // abstract

} // oln

#endif // ! OLENA_CORE_ABSTRACT_QITER_HH
