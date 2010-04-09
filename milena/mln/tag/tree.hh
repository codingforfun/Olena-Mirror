// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TAG_TREE_HH
# define MLN_TAG_TREE_HH

namespace mln
{

  namespace tag
  {

    namespace tree
    {

      template <typename T>
      struct tree_t : Object<T> { protected: tree_t(){} };
      struct max_t : tree_t<max_t> {};
      struct min_t : tree_t<min_t> {};

      static max_t max;
      static min_t min;

    } // end of namespace mln::tag::tree

  } // end of namespace mln::tag

} // end of namespace mln

#endif // !MLN_TAG_TREE_HH
