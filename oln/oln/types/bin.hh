// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_BIN_HH
# define OLENA_VALUE_BIN_HH

# include <oln/core/contract.hh>
# include <oln/types/rec_value.hh>
# include <oln/types/typetraits.hh>

// FIXME: optraits_bin.hh is included at the end of the file.

namespace oln
{

  //
  //  Typetraits
  //
  ///////////////

  template <>
  struct typetraits<bin>
  {
    typedef bin				self;
    typedef optraits<self>		optraits;

    typedef self			base_type;
    typedef bool			storage_type;
    typedef bin				signed_type;
    typedef bin				unsigned_type;
    typedef bin				cumul_type;
    typedef bin				largest_type;
    typedef bin				signed_largest_type;
    typedef bin				signed_cumul_type;
    typedef bin				unsigned_largest_type;
    typedef bin				unsigned_cumul_type;
    typedef unsigned int		integer_type;

    typedef self op_traits;
  };


  namespace type_definitions
  {
    //
    //  Class bin
    //
    //////////////


    class bin : public rec_enum<bin>
    {
    public:
      bin () { _value = 0; }

      bin (unsigned char val)
      {
	precondition (val < 2);
	_value = val;
      }

      bin& operator=(unsigned char val)
      {
	precondition (val < 2);
	_value = val;
	return *this;
      }

      template <class T>
      bin (const rec_scalar<T>& val)
      {
	precondition (val < 2);
	_value = val;
      }
      template <class T>
      bin& operator=(const rec_scalar<T>& val)
      {
	precondition (val < 2);
	_value = val;
	return *this;
      }

      operator unsigned char() const { return _value; }
    };

    inline std::ostream&
    operator<<(std::ostream& stream, const oln::bin& rhs)
    {
      stream << (unsigned int) rhs.value();
      return stream;
    }

  } // type_definitions

} // end of namespace oln

// FIXME: find another solution to allow self contained bin.hh
# include <oln/types/optraits_bin.hh>

#endif // ndef OLENA_VALUE_BIN_HH
