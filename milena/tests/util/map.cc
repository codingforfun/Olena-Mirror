// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/util/map.hh>

const char *keys[] = { 0, "1", "2", "3", "4" };

int main ()
{
  using namespace mln;
  typedef util::map<std::string, unsigned> map_t;
  map_t m;

  m["1"] = 1;
  m["2"] = 2;
  m["3"] = 3;
  m["4"] = 4;

  mln_assertion(m.nelements() == 4);

  mln_assertion(m.has("3"));
  m.erase("3");
  mln_assertion(!m.has("3"));
  m.clear();
  mln_assertion(m.is_empty());

  {
    unsigned i = 1;
    mln_eiter(map_t) e(m);
    for_all(e)
    {
      mln_assertion(keys[i] == e.get_subject().first);
      mln_assertion(i++ == e.get_subject().second);
    }
  }

  {
    unsigned i = 4;
    mln_bkd_eiter(map_t) e(m);
    for_all(e)
    {
      mln_assertion(keys[i] == e.get_subject().first);
      mln_assertion(i-- == e.get_subject().second);
    }
  }
}
