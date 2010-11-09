/* Copyright (C) 2005, 2009, 2010 EPITA Research and Development
   Laboratory (LRDE).

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#include "my_lib/damien.hh"

#include "xtc-all.hh"

using namespace xtc::language;

namespace xtc
{
  fun down("mk_down");

  template <typename T>
  data mk_down(const T& t)
  {
    fun ctor(std::string("down< ") + mlc_name_of(t) + " >");
    return ctor(t);
  }
}

int main()
{
  xtc::include_dir(XTC_FIXTURES);
  xtc::include("my_lib/damien.hxx");

  down<char>               a('x');
  down<int>                b(10);
  std::string              bar("bar"); 
  down<std::string>        c(bar); // c("bar"); // it's worst
  down<down<std::string> > d(c);
  down<int>                e(20);

  var d2 = d;

  std::cout << d2 << std::endl;

  fun  xtc_foo("foo");
  fun  xtc_change("change");
  ctor mk_down_char("down<char>");
  ctor mk_down_int("down<int>");
  meth xtc_print_noarg("print_noarg");
  meth xtc_get_i("get_i");
  meth xtc_get_t("get_t");
  method xtc_clone("clone"); // Same as meth

  var f = mk_down_char('x');

  std::cout << mlc_name_of(f) << std::endl;

  var g = mk_down_int(44);
  var h = xtc::mk_down(e);

  var j = xtc::down(46);

  meth j_print_noarg = j.method("print_noarg");

  // std::cout is not printable
  // but a data containing std::cout yes
  var xtc_std_cout = std::cout;
  std::cout << xtc_std_cout << std::endl;

  for ( int i = 0; i < 5; ++i )
  {
    std::cout << "*** Turn " << i << " ***" << std::endl;
    xtc_print_noarg(a);

    xtc_foo(a);

    xtc_foo(b);
    xtc_foo(c);
    xtc_foo(d);
    xtc_foo(e);
    xtc_foo(42);

    xtc_print_noarg(f);

    xtc_foo(g);

    xtc_change(c);

    var x1 = xtc_get_i(b);
    std::cout << "xtc_get_i(b) => " << x1 << std::endl;
    var x2 = *xtc_get_t(d);
    std::cout << "xtc_get_t(d) => " << x2 << std::endl;

    var x3 = *xtc_clone(a);
    std::cout << "*clone(a) => " << x3 << std::endl;

    j.send("print_noarg");

    j.fake_method(a).send("print_noarg");
    h.fake_method(a).send("print_noarg");

    j_print_noarg();
  }

  std::cout << "exiting" << std::endl;
}
