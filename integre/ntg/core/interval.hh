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

#ifndef NTG_INTERVAL_HH
# define NTG_INTERVAL_HH

# include <ntg/config/system.hh>
# include <ntg/core/type_traits.hh>

# include <string>
# include <sstream>

namespace ntg
{

  template <class T, T i_min, T i_max>
  class bounded
  {
  public:
    typedef T storage_type;

    static T min() { return i_min; }
    static T max() { return i_max; }
    static T inf() { return i_min; }
    static T sup() { return i_max; }

    static std::string name()
    {
      std::ostringstream out;
      out << "bounded<" << type_traits<T>::name() << ", " << i_min 
	  << ", " << i_max << ">"<< std::ends;
      return out.str();
    }
  };

  template <unsigned i_min, unsigned i_max>
  class bounded_u : public bounded<unsigned, i_min, i_max>
  {
  public:
    static std::string name()
    {
      std::ostringstream out;
      out << "bounded_u<" << i_min << ", " << i_max << ">"<< std::ends;
      return out.str();
    }
  };

  template <signed i_min, signed i_max>
  class bounded_s : public bounded<signed, i_min, i_max>
  {
  public:
    static std::string name()
    {
      std::ostringstream out;
      out << "bounded_s<" << i_min << ", " << i_max << ">"<< std::ends;
      return out.str();
    }
  };

} // end of ntg

#endif // ndef NTG_INTERVAL_HH
