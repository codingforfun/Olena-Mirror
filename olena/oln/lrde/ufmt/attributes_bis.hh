// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LRDE_UFMT_ATTRIBUTES_BIS_HH
# define OLENA_LRDE_UFMT_ATTRIBUTES_BIS_HH



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      struct longest_leaf_path_t // longest path to a leaf
      {
	typedef longest_leaf_path_t self_t;

	template <class I, class P>
	void init(const I&, const P&)
	{
	  value = 0;
	}
	template <class I, class P>
	void insert(const I& f, const P& p)
	{
	  // FIXME: Broken assertion.
// 	  assert(f[p] == h);
	}
	void embrace(const self_t& rhs)
	{
	  // FIXME: Broken assertion.
// 	  assert(rhs.h > h);
	  if (rhs.value + 1 > value)
	    value = rhs.value + 1;
	}
	operator unsigned() const
	{
	  return value;
	}

      private:
	unsigned value;
      };


    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_ATTRIBUTES_BIS_HH
