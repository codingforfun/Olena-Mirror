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

#ifndef OLENA_CORE_IMAGEND_SIZE_HH
# define OLENA_CORE_IMAGEND_SIZE_HH

# include <oln/core/image_size.hh>
# include <oln/core/coord.hh>

namespace oln {

  template< unsigned Dim, class Inferior = type::bottom >
  class imagend_size : public image_size< imagend_size<Dim, Inferior> >
  {
  public:

    typedef Inferior inferior;

    enum { dim = Dim };

    coord nth(unsigned n) const
    {
      return _coord[n];
    }

    template< class S >
    bool operator==(const imagend_size<Dim, S>& size) const
    {
      for (unsigned i = 0; i < Dim; ++i)
	if (_coord[i] != size._coord[i])
	  return false;
      return true;
    }

    template< class S >
    bool operator!=(const imagend_size<Dim, S>& size) const
    {
      for (unsigned i = 0; i < Dim; ++i)
	if (_coord[i] != size._coord[i])
	  return true;
      return false;
    }

    static std::string name()
    {
      std::ostringstream out;
      out << "imagend_size<" << dim << ","
          << typename_of<Inferior>() << ">" << std::ends;
      return out.str();
    }

  protected:
    coord& nth(unsigned n)
    {
      return _coord[n];
    }

  private:
    coord _coord[dim];
  };

} // end of oln

#endif // ! OLENA_CORE_IMAGEND_SIZE_HH
