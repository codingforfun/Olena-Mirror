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

#ifndef METALIC_SWITCH_HH
# define METALIC_SWITCH_HH

# include <mlc/bool.hh>
# include <mlc/if.hh>


namespace mlc
{


  // FIXME: not yet cleaned.

  struct invalid {};

  template<unsigned Cond, class Ret, class Cases = invalid>
  struct case_ {};

  template<unsigned Cond, class Cases, class Default = invalid>
  struct switch_;

  template<unsigned Cond, unsigned Compare, class Ret, class Default>
  struct switch_<Cond, case_<Compare, Ret>, Default >
  {
    typedef typename if_< value<bool,(Cond == Compare)>, Ret, Default >::ret ret;
  };

  template<unsigned Cond,
      unsigned Compare,
      class Ret,
      class Cases,
      class Default>
  struct switch_<Cond, case_<Compare, Ret, Cases>, Default >
  {
    typedef typename
      if_< value<bool, bool(Cond == Compare)>,
	   Ret,
	   typename switch_<Cond, Cases, Default>::ret>::ret ret;
  };

  template<bool Cond, class Ret, class Cases = invalid>
  struct bool_case_ {};

  template<class Cases, class Default = invalid>
  struct bool_switch_;

  template<bool Cond, class Ret, class Default>
  struct bool_switch_<bool_case_<Cond, Ret>, Default >
  {
    typedef typename if_< value<bool,Cond>, Ret, Default >::ret ret;
  };

  template<bool Cond,class Ret, class Cases, class Default>
  struct bool_switch_<bool_case_<Cond, Ret, Cases>, Default >
  {
    typedef typename
    if_< value<bool, Cond>,
	 Ret,
	 typename bool_switch_<Cases, Default>::ret >::ret ret;
  };

} // end of namespace mlc


#endif // ! METALIC_SWITCH_HH
