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

#ifndef OLENA_CORE_ABSTRACT_ITER_HH
# define OLENA_CORE_ABSTRACT_ITER_HH

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln
{
  namespace abstract
  {
    template<class Exact>
    struct iter; // fwd_decl
  } // end of abstract

  template<class Exact>
  struct iter_traits;

  template<class Exact>
  struct iter_traits<abstract::iter<Exact> >
  {

  };

  // shortcuts
  using mlc::_begin;
  using mlc::begin;
  using mlc::_end;
  using mlc::end;
  
  namespace abstract 
  {
    // iter
    
    template<class Exact>
    struct iter : public mlc::any<Exact>
    {

      typedef typename iter_traits<Exact>::point_type point_type;
      typedef typename iter_traits<Exact>::dpoint_type dpoint_type;


      const point_type& point_ref() const
      {
	return p_;
      }

      bool operator==(const abstract::point<point_type>& p) const
      {
	return p_ == p.exact();
      }

      bool operator!=(const abstract::point<point_type>& p) const
      {
	return p_ != p.exact();
      }

      point_type operator+(const abstract::dpoint<dpoint_type>& dp) const
      {
	precondition(*this != end);
	return p_ + dp.exact();
      }

      point_type operator-(const abstract::dpoint<dpoint_type>& dp) const
      {
	precondition(*this != end);
	return p_ - dp.exact();
      }

      operator point_type() const
      {
	return exact().to_point();
      }

      // it's convenient to type `it.cur()' instead of `(point)it' when
      // necessary.
      point_type cur() const
      {
	return *this;
      }

      // deferred methods are:
      //
      //   void _goto_begin();
      //   void _goto_end();
      //   bool _is_at_end() const;
      //   void _goto_next();

      mlc::_begin operator=(mlc::_begin b)
      {
	exact().goto_begin_();
	return b;
      }

      mlc::_end operator=(mlc::_end e)
      {
	exact().goto_end_();
	return e;
      }

      bool operator==(mlc::_end) const
      {
	return exact().is_at_end_();
      }

      void operator++()
      {
	precondition(*this != end);
	exact().goto_next_();
      }

//       typename mlc::exact<self>::ret operator++(int)
//       {
// 	precondition(*this != end);
// 	typename mlc::exact<self>::ret tmp = to_exact(*this);
// 	this->operator++();
// 	return tmp;
//       }

      // deduced methods:

      bool operator!=(mlc::_end e) const
      {
	return ! this->operator==(e);
      }


      static std::string name() { return std::string("iter<") +
				    Exact::name() + ">"; }
    protected:
      point_type p_;

      iter() {}
    };
        
  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER_HH
