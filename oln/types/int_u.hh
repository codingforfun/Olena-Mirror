// Copyright 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_INT_U_HH
# define OLENA_VALUE_INT_U_HH

# include <oln/config/system.hh>
# include <oln/meta/cmp.hh>

# include <oln/types/rec_value.hh>
# include <oln/types/behaviour.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/builtins_properties.hh>

// Macros //

# define INT_U_CTOR_FROM_BUILTIN_INT(Builtin)   \
int_u (const Builtin rhs)			\
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

  template <unsigned nbits, class behaviour> class int_u; // fwd_decl

  //
  //  Typetraits
  //
  ///////////////

  template <unsigned nbits, class behaviour>
  struct typetraits<int_u<nbits, behaviour> >
  {
    typedef int_u<nbits, behaviour> self;
    typedef optraits<self> optraits;

    typedef self base;
    // FIXME: calculate it from nbits
    typedef typename C_for_int_u<nbits>::type store;

    typedef int_u<32, strict> cumul;

    // internal use, useful for decorators
    typedef self op_traits;

    // FIXME: add missing
  };


  namespace type_definitions
  {

    //
    //  Class int_u<Nbits, Behaviour>
    //
    //////////////////////////////////

    template <unsigned nbits, class behaviour>
    class int_u : public rec_int_u<int_u<nbits, behaviour> >
    {
      typedef int_u<nbits, behaviour> self;
      typedef typename typetraits<self>::store store_type;
      // dev note : should be directly optraits<self_t>, but with g++ this
      // breaks inheritance in optraits herarchy ...
      typedef typename typetraits<self>::optraits optraits_type;

    public:
      int_u ()
      {
	_value = 0;
      }

      // We define ctor for each builtin to avoid implicit builtin promotion

      INT_U_CTOR_FROM_BUILTIN_INT(unsigned long);
      INT_U_CTOR_FROM_BUILTIN_INT(signed   long);

      INT_U_CTOR_FROM_BUILTIN_INT(unsigned int);
      INT_U_CTOR_FROM_BUILTIN_INT(signed   int);

      INT_U_CTOR_FROM_BUILTIN_INT(unsigned short);
      INT_U_CTOR_FROM_BUILTIN_INT(signed   short);

      INT_U_CTOR_FROM_BUILTIN_INT(unsigned char);
      INT_U_CTOR_FROM_BUILTIN_INT(signed   char);

      int_u (const float rhs)
      {
	_value = optraits_type::check(truncf(rhs));
      }
      self& operator=(const float rhs)
      {
	_value = optraits_type::check(truncf(rhs));
	return *this;
      }

      int_u (const double rhs)
      {
	_value = optraits_type::check(trunc(rhs));
      }
      self& operator=(const double rhs)
      {
	_value = optraits_type::check(trunc(rhs));
	return *this;
      }


      // FIXME: add int_u<mbits> here, and check only if mbits > nbits
      int_u (const self& rhs)
      {
	_value = rhs.value();
      }
      self& operator=(const self& rhs)
      {
	_value = rhs.value();
	return *this;
      }

      template <class T>
      int_u (const rec_scalar<T>& rhs)
      {
	_value = optraits_type::check(rhs.value());
      }
      template <class T>
      self& operator=(const rec_scalar<T>& rhs)
      {
	_value = optraits_type::check(rhs.value());
	return *this;
      }

      // Cast
      operator store_type () const { return _value; }

    private:
      // We want to prevent this
      int_u(bool);
    };

  } // type_definitions
} // end of namespace oln

#endif // ndef OLENA_VALUE_INT_U_HH
