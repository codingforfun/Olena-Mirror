// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_DATA_STORAGE_HH
# define OLENA_CORE_ABSTRACT_DATA_STORAGE_HH

# include <mlc/any.hh>

# include <oln/core/macros.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct data_storage : public mlc::any__best_speed<E>
    {
      // abstract methods

      bool has_data() const
      {
	return this->exact().impl_has_data();
      }

      void clear_data()
      {
	if (! this->has_data())
	  return;
	this->exact().impl_clear_data();
	postcondition(! this->has_data());
      }

      const oln_size_type(E)& size() const
      {
	return this->exact().impl_size();
      }

      void resize(const oln_size_type(E)& s)
      {
	this->exact().impl_resize(s);
      }

      unsigned long npoints() const
      {
	if (! this->has_data())
	  return 0;
	return this->exact().impl_npoints();
      }

      bool hold(const oln_point_type(E)& p) const
      {
	if (! this->has_data())
	  return false;
	return this->exact().impl_hold(p);
      }

      const oln_data_type(E) get(const oln_point_type(E)& p) const
      {
	precondition(this->has_data());
	precondition(this->hold_large(p));
	return this->exact().impl_get(p);
      }

      void set(const oln_point_type(E)& p,
	       const oln_data_type(E)& v)
      {
	precondition(this->has_data());
	precondition(this->hold_large(p));
	this->exact().impl_set(p, v);
      }

      void set_data(const oln_data_type(E)& v)
      {
	precondition(this->has_data());
	this->exact().impl_set_data(v);
      }

      // polymorphic method with default

      bool hold_large(const oln_point_type(E)& p) const
      {
	if (! this->has_data())
	  return false;
	return this->exact().impl_hold_large(p);
      }

      bool impl_hold_large(const oln_point_type(E)& p) const
      {
	return this->exact().impl_hold(p);
      }

    protected:
      data_storage() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_DATA_STORAGE_HH
