// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_COMPARE_HH
# define OLENA_LEVEL_COMPARE_HH

# include <oln/basics.hh>

namespace oln {
  namespace level {

    template<class I1, class I2> inline
    bool is_greater_or_equal(const abstract::image<I1>& input1,
			     const abstract::image<I2>& input2)
    {
      precondition(input1.size() == input2.size());
     oln_iter_type(I1) p(input1);
      for_all (p)
	if (!(input1[p] >= input2[p]))
	  return false;
      return true;
    }

    template<class I1, class I2> inline
    bool is_greater(const abstract::image<I1>& input1,
		    const abstract::image<I2>& input2)
    {
      precondition(input1.size() == input2.size());
     oln_iter_type(I1) p(input1);
      for_all (p)
	if (!(input1[p] > input2[p]))
	  return false;
      return true;
    }

    template<class I1, class I2> inline
    bool is_lower_or_equal(const abstract::image<I1>& input1,
			   const abstract::image<I2>& input2)
    {
      precondition(input1.size() == input2.size());
     oln_iter_type(I1) p(input1);
      for_all (p)
	if (!(input1[p] <= input2[p]))
	  return false;
      return true;
    }

    template<class I1, class I2> inline
    bool is_lower(const abstract::image<I1>& input1,
		  const abstract::image<I2>& input2)
    {
      precondition(input1.size() == input2.size());
     oln_iter_type(I1) p(input1);
      for_all (p)
	if (!(input1[p] < input2[p]))
	  return false;
      return true;
    }

    template<class I1, class I2> inline
    bool is_equal(const abstract::image<I1>& input1,
		  const abstract::image<I2>& input2)
    {
      precondition(input1.size() == input2.size());
     oln_iter_type(I1) p(input1);
      for_all (p)
	if (!(input1[p] == input2[p]))
	  return false;
      return true;
    }

  } // end of level
} // end of oln

#endif // OLENA_LEVEL_COMPARE_HH
