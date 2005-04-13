// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

# include <mlc/any.hh>
# include <mlc/contract.hh>

# include <oln/core/typedefs.hh>



# define for_all(i) \
  for(i.start(); i.is_valid(); i.next())

# define for_all_remaining(i) \
  for(; i.is_valid(); i.next())




namespace oln {

  namespace abstract {

    template <typename E>
    struct iter : public mlc::any<E>
    {

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

      void invalidate()
      {
	this->exact().impl_invalidate();
	postcondition(! this->is_valid());
      }

    protected:

      iter() {}

      ~iter()
      {
	mlc_check_method_impl(E, void, start, , );
	mlc_check_method_impl(E, void, next, , );
	mlc_check_method_impl(E, bool, is_valid, , const);
	mlc_check_method_impl(E, void, invalidate, , );
      }

    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_ITER_HH
