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

#ifndef OLENA_LEVEL_LUT_HH
# define OLENA_LEVEL_LUT_HH

# include <oln/basics.hh>

# include <map>

namespace oln {

  namespace level {

    // few values -> new values
    // otherwise  -> id

    template<class T, class T2 = T>
    class hlut
    {
      typedef std::map<T,T2> hmap_t;

    public:
      typedef T2 output_t;

      hlut()
      {}

      hlut& 
      set(const T& val, const T2& newval)
      {
	hmap_[val] = newval;
	return *this;
      }

      const T2 
      operator()(const T& val) const
      {
	static typename hmap_t::const_iterator i;
	i = hmap_.find(val);
	return i != hmap_.end() ? i->second : val;
      }

    private:
      hmap_t hmap_;
    };


    // few values -> new values
    // otherwise  -> default value

    template<class T, class T2 = T>
    class hlut_def
    {
      typedef std::map<T,T2> hmap_t;

    public:
      typedef T2 output_t;

      hlut_def()
      {
	defaultval_ = T2();
      }

      hlut_def&
      set(const T& val, const T2& newval)
      {
	hmap_[val] = newval;
	return *this;
      }

      hlut_def& 
      set_default(const T2& defaultval)
      {
	defaultval_ = defaultval;
	return *this;
      }

      const T2 
      operator()(const T& val) const
      {
	static typename hmap_t::const_iterator i;
	i = hmap_.find(val);
	return i != hmap_.end() ? i->second : defaultval_;
      }

    private:
      hmap_t hmap_;
      T2 defaultval_;
    };

  } // end of namespace level

} // end of namespace oln

#endif // ! OLENA_LEVEL_LUT_HH
