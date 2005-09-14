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

#ifndef OLENA_CORE_PW_CHECK_HH
# define OLENA_CORE_PW_CHECK_HH

# include <mlc/cmp.hh>
# include <oln/core/abstract/image_typeness.hh>
# include <oln/core/pw/image.hh>


namespace oln {

  namespace pw {

    template <typename F>
    bool check(const abstract::function<F>& pred)
    {
      mlc::eq< oln_typeness_of(oln_pw_type_of(F, value)), typeness::binary_tag >::ensure();
      typedef pw::image<F> I;
      I ima(pred);
      oln_type_of(I, fwd_piter) p(ima.size());
      for_all_p (p)
	if (not ima[p])
	  return false;
      return true;
    }

  } // end of namespace oln::pw

  template <typename F>
  bool pw_check(const pw::abstract::function<F>& pred)
  {
    return pw::check(pred);
  }

} // end of namespace oln


#endif // ! OLENA_CORE_PW_CHECK_HH
