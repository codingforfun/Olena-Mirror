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

#ifndef OLENA_CORE_DPOINTND_HH
# define OLENA_CORE_DPOINTND_HH

# include <oln/core/coord.hh>
# include <oln/core/point.hh>
# include <iostream>

namespace oln {

  // fwd decl
  template< unsigned Dim, class Inferior >
  class pointnd;

  template< unsigned Dim, class Inferior = type::bottom >
  class dpointnd : public dpoint< dpointnd< Dim, Inferior > >
  {
  public:
    typedef Inferior inferior;
    typedef dpointnd self;

    enum { dim = Dim };

    dpointnd()
    {
    }

    template< class I >
    explicit dpointnd(const pointnd< Dim, I >& p)
    {
      for (unsigned i = 0; i < dim; ++i)
	nth(i) = p.nth(i);
    }

    coord nth(const unsigned dim) const
    {
      return _coord[dim];
    }

    coord& nth(const unsigned dim)
    {
      return _coord[dim];
    }

    bool operator==(const self& p) const
    {
      for (unsigned i = 0; i < dim; ++i)
	if (p.nth(i) != nth(i))
	  return false;
      return true;
    }

    bool operator!=(const self& p) const
    {
      for (unsigned i = 0; i < dim; ++i)
	if (p.nth(i) != nth(i))
	  return true;
      return false;
    }

    bool is_centered(void) const
    {
      for (unsigned i = 0; i < dim; ++i)
	if (nth(i) != 0)
	  return false;
      return true;
    }


    static std::string name()
    {
      std::ostringstream out;
      out << "dpointnd<" << dim << ","
	  << typename_of<Inferior>() << ">" << std::ends;
      return out.str();
    }

  private:
    coord _coord[dim];
  };

  namespace internal
  {
    template<unsigned Dim, class Inferior>
    struct default_less< dpointnd<Dim, Inferior> >
    {
      bool operator()(const dpointnd<Dim, Inferior>& l,
		      const dpointnd<Dim, Inferior>& r) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (l.nth(i) < r.nth(i))
	    return true;
	  else if (l.nth(i) > r.nth(i))
	    return false;
	return false;
      }
    };

  } // internal

} // oln

#endif // ! OLENA_CORE_DPOINTND_HH
