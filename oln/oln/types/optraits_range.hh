// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPTRAITS_RANGE_HH
# define OLENA_VALUE_OPTRAITS_RANGE_HH

# include <oln/types/range.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/global_ops_traits.hh>

# include <string>
# include <sstream>

namespace oln
{

  template<class T, class interval, class behaviour>
  struct optraits<range<T, interval, behaviour> > : public optraits<T>
  {
  public:
    typedef range<T, interval, behaviour> self;

  private:
    typedef typename typetraits<self>::storage_type storage_type;
    typedef typename behaviour::get<self> behaviour_type;
    typedef typename interval::storage_type interval_type;

  public:
    // behaviour's check

    template <class P>
    static storage_type check(const P& rhs)
    { return behaviour_type::apply(rhs); }

    //
    //  Properties
    //
    ////

    static interval_type min()
    { return interval::min(); }
    
    static interval_type max()
    { return interval::max(); }

    static interval_type inf()
    { return interval::inf(); }
    
    static interval_type sup()
    { return interval::sup(); }
    
    // debug
    static std::string name() 
    {
      std::ostringstream out;
      out << "range<" << optraits<T>::name() << ", " << interval::name() 
	  << ", " << behaviour::name() << ">"<< std::ends;
      return out.str();
    }
  };
  

  //
  //  dev note
  //
  //  Arithmetic and other binary operators use base_type, 
  //  check typetraits<range>::op_traits
  //
  
} // end of namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_RANGE_HH
