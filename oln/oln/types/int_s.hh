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

#ifndef OLENA_VALUE_INT_S_HH
# define OLENA_VALUE_INT_S_HH

# include <oln/config/system.hh>
# include <oln/meta/cmp.hh>
# include <oln/types/builtins_properties.hh>
# include <oln/types/predecls.hh>
# include <oln/types/rec_value.hh>
# include <oln/types/behaviour.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>

// FIXME: optraits_int_s.hh is included at the end of the file.

// Macros //

// FIXME: add bits number comparison to avoid useless checks.

# define INT_S_CTOR_FROM_BUILTIN(Builtin)       \
int_s (const Builtin rhs)			\
{						\
  _value = optraits_type::check(rhs);		\
}						\
self& operator=(const Builtin rhs)	        \
{						\
  _value = optraits_type::check(rhs);		\
  return *this;					\
}

// --- //

namespace oln
{

  //
  //  Typetraits
  //
  ///////////////

  template <unsigned nbits, class behaviour>
  struct typetraits<int_s<nbits, behaviour> >
  {
    typedef int_s<nbits, behaviour>		self;
    typedef optraits<self>			optraits;
    typedef typename behaviour::get<self>	behaviour_type;

    typedef self				base_type;
    typedef typename C_for_int_s<nbits>::type	storage_type;
    typedef self				signed_type;
    typedef int_u<nbits-1, behaviour>		unsigned_type;
    // FIXME: calculate it more precisely
    typedef int_s<32, behaviour>		cumul_type;
    typedef int_s<32, behaviour>		largest_type;
    typedef int_s<32, behaviour>		signed_largest_type;
    typedef int_s<32, behaviour>		signed_cumul_type;
    typedef int_u<32, behaviour>		unsigned_largest_type;
    typedef int_u<32, behaviour>		unsigned_cumul_type;
    typedef signed int				integer_type;

    // internal use, useful for decorators
    typedef self op_traits;
  };


  namespace type_definitions
  {

    //
    //  Class int_s<Nbits, Behaviour>
    //
    //////////////////////////////////


    template <unsigned nbits, class behaviour>
    class int_s : public rec_int_s<int_s<nbits, behaviour> >
    {
      typedef int_s<nbits, behaviour> self;
      typedef typename typetraits<self>::storage_type storage_type;
      typedef typename typetraits<self>::optraits optraits_type;

    public:

      int_s () { _value = 0; }

      // We define ctor for each builtin to avoid implicit builtin promotion

      INT_S_CTOR_FROM_BUILTIN(unsigned long);
      INT_S_CTOR_FROM_BUILTIN(signed   long);

      INT_S_CTOR_FROM_BUILTIN(unsigned int);
      INT_S_CTOR_FROM_BUILTIN(signed   int);

      INT_S_CTOR_FROM_BUILTIN(unsigned short);
      INT_S_CTOR_FROM_BUILTIN(signed   short);

      INT_S_CTOR_FROM_BUILTIN(unsigned char);
      INT_S_CTOR_FROM_BUILTIN(signed   char);

      template <unsigned mbits, class B2>
      int_s (const int_s<mbits, B2>& rhs)
      {
	if (mbits <= nbits)
	  _value = rhs.value();
	else
	  _value = optraits_type::check(rhs.value());
      }
      template <unsigned mbits, class B2>
      self& operator=(const int_s<mbits, B2>& rhs)
      {
	if (mbits <= nbits)
	  _value = rhs.value();
	else
	  _value = optraits_type::check(rhs.value());
	return *this;
      }

      template <class T>
      int_s (const rec_scalar<T>& rhs)
      {
	_value = optraits_type::check(rhs.value());
      }
      template <class T>
      self& operator=(const rec_scalar<T>& rhs)
      {
	_value = optraits_type::check(rhs.value());
	return *this;
      }

      int_s (const float rhs)
      {
	_value = optraits_type::check(roundf(rhs));
      }
      self& operator=(const float rhs)
      {
	_value = optraits_type::check(roundf(rhs));
	return *this;
      }

      int_s (const double rhs)
      {
	_value = optraits_type::check(round(rhs));
      }
      self& operator=(const double rhs)
      {
	_value = optraits_type::check(round(rhs));
	return *this;
      }

      operator storage_type () const { return _value; }

    private:
      // We want to prevent this
      int_s(bool);
    };

    template<unsigned nbits, class behaviour>
    inline std::ostream&
    operator<<(std::ostream& stream, const oln::int_s<nbits, behaviour>& rhs)
    {
      stream << signed(rhs.value());
      return stream;
    }

  } // type_definitions
} // end of namespace oln

// FIXME: find another solution if we want self contained int_s.hh
# include <oln/types/optraits_int_s.hh>

#endif // ndef OLENA_VALUE_INT_S_HH
