// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_META_CONTROL_HH
# define OLENA_META_CONTROL_HH

# include "meta/cmp.hh"


namespace oln {

  namespace meta {


    namespace internal {

      template<int i, template<int,int>class Cmp, int to, int by, bool do_again>
      struct simple_for_statement;

      template<int i, template<int,int>class Cmp, int to, int by>
      struct simple_for_statement<i, Cmp, to, by, true>
      {
	template<class F>
	static F& exec(F& f)
	{
	  f.template exec<i>();
	  return simple_for_statement<i+by, Cmp, to, by, Cmp<i+by,to>::ret>::exec(f);
	}
      };

      template<int i, template<int,int>class Cmp, int to, int by>
      struct simple_for_statement<i, Cmp, to, by, false>
      {
	template<class F>
	static F& exec(F& f)
	{
	  return f;
	}
      };

    } // end of internal



    // simple_for_<from, Cmp,to, by>::exec(f);
    //
    // mimics:
    //
    // simple_for (i = from; Cmp(i, to)::ret; i += by)
    //   f::exec<i>();

    template<int from, template<int,int>class Cmp, int to, int by>
    struct simple_for_
    {
      template<class F>
      static F& exec(F& f)
      {
	return internal::simple_for_statement< from, Cmp, to, by,
	                                       Cmp<from,to>::ret >::exec(f);
      }
    };


  } // end of meta

} // end of oln


#endif // ! OLENA_META_CONTROL_HH
