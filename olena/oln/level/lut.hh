// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

    /*! \brief Look up table "id" version.
    **
    ** If the value has not been set, return id (see the example).
    **
    ** \see hlut_def
    ** \verbatim
    ** hlut<int> l;
    **
    ** l.set(16, 64);
    **
    ** cout << l(16) << ","   // print 64,
    **      << l(51) << endl; // print 51
    ** \endverbatim
    */
    template<class T, class T2 = T>
    class hlut
    {
      typedef std::map<T,T2> hmap_t;

    public:
      typedef T2 output_t;

      hlut()
      {}

      /*! Register a value.
      **
      ** \arg val Key.
      ** \arg newval Value corresponding to the key.
      */
      hlut&
      set(const T& val, const T2& newval)
      {
	hmap_[val] = newval;
	return *this;
      }

      /*! Get the value corresponding to the key.
      **
      ** Return the key if the key has not been set.
      **/
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


    /*! \brief Look up table "default" version.
    **
    ** If the value has not been set, return the default value.
    ** \note The default value can be redefined.
    **
    ** \see hlut_def
    ** \verbatim
    ** hlut_def<int> l;
    **
    ** l.set(16, 64);
    **
    ** cout << l(16) << ","   // print "64,"
    **      << l(51) << endl; // print "0"
    ** l.set_default(42);
    ** cout << l(51) << endl; // print "42"
    ** \endverbatim
    */
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

      /*! Register a value.
      **
      ** \arg val Key.
      ** \arg newval Value corresponding to the key.
      */
      hlut_def&
      set(const T& val, const T2& newval)
      {
	hmap_[val] = newval;
	return *this;
      }

      //! Set a default value.
      hlut_def&
      set_default(const T2& defaultval)
      {
	defaultval_ = defaultval;
	return *this;
      }

      /*! Get the value corresponding to the key.
      **
      ** Return defaultval_ if the key has not been set.
      */
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
