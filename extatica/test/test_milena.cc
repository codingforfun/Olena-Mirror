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

#include <cstdlib>

#include "xtc-all.hh"
// FIXME: This is not elegant: this test is not in test/wrappers/, yet
// it includes a header from src/wrappers/.
#include "wrappers/mln-config.hh"

using namespace xtc::language;

int main()
{
  char* env_milena_include_path = std::getenv("MILENA_INCLUDE_PATH");
  const char* milena_include_path =
    env_milena_include_path ? env_milena_include_path : MILENA_INCLUDE_PATH;
  xtc::include_path(milena_include_path);
  xtc::include("mln/core/image/image2d.hh");
  xtc::include("mln/data/fill.hh");
  xtc::include("mln/debug/iota.hh");
  xtc::include("mln/debug/println.hh");

  ctor mk_image2d_int("mln::image2d<int>");
  fun fill("mln::data::fill");
  fun iota("mln::debug::iota");
  fun println("mln::debug::println");

  var ima = mk_image2d_int(3, 3);
  fill(ima, 0);
  println("ima (before) =", ima);
  iota(ima);
  println("ima (after) =", ima);
}
