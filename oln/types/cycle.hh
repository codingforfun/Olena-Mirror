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

#ifndef OLENA_VALUE_CYCLE_HH
# define OLENA_VALUE_CYCLE_HH

# include <oln/meta/type.hh>

# include <oln/types/rec_value.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/optraits_scalar.hh>
# include <oln/types/interval.hh>
# include <oln/types/behaviour.hh>

namespace oln
{

  //
  //  Typetraits
  //
  ///////////////

  template <class T, class interval>
  struct typetraits<cycle<T, interval> >
  {
    typedef cycle<T, interval> self;
    typedef optraits<self> optraits;
    typedef cycle_behaviour::get<self> behaviour_type;

    typedef typename typetraits<T>::base_type		base_type;
    typedef T						storage_type;
    typedef typename typetraits<T>::signed_type		signed_type;
    typedef typename typetraits<T>::unsigned_type	unsigned_type;
    typedef self					cumul_type;
    typedef self					largest_type;
    typedef self					signed_largest_type;
    typedef self					signed_cumul_type;
    typedef self					unsigned_largest_type;
    typedef self					unsigned_cumul_type;
    typedef typename typetraits<T>::integer_type	integer_type;


    // internal type used for binary operations traits
    typedef typename typetraits<T>::base_type op_traits;
  };


  namespace type_definitions
  {
    //
    //  Class cycle<DecoratedType, class interval>
    //
    //  The interval has 0
    //
    ////////////////////////////////////////////////////

    //
    //  dev note
    //
    //  Arithmetic and other binary operators use base_type,
    //  check typetraits<cycle>::op_traits
    //

    template <class T,
	      class interval>
    class cycle : public rec_scalar<cycle<T, interval> >
    {
    public:
      typedef cycle<T, interval> self;

    private:
      // shortcuts
      typedef typename typetraits<self>::optraits optraits_type;
      typedef typename typetraits<self>::base_type base_type;
      typedef typename typetraits<base_type>::storage_type base_storage_type;

    public:
      cycle () { _value = 0; }

      template <class U>
      cycle (const U& u)
      {
	is_a(optraits<U>, optraits_scalar)::ensure();
	_value = optraits_type::check(u);
      }
      template <class U>
      self& operator=(const U& u)
      {
	_value = optraits_type::check(u);
	return *this;
      }

      // cast
      operator base_storage_type() const { return _value; }
    };

    template<class T, class interval>
    inline std::ostream&
    operator<<(std::ostream& stream, const oln::cycle<T, interval>& rhs)
    {
      // Cast useful for cycle<unsigned char, ...>
      stream << (typename typetraits<T>::largest_type)(rhs.value());
      return stream;
    }

  } // type_definitions

} // end of namespace oln

#endif // ndef OLENA_VALUE_CYCLE_HH
