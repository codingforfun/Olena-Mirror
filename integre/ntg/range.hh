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

#ifndef OLENA_VALUE_RANGE_HH
# define OLENA_VALUE_RANGE_HH

# include <mlc/type.hh>

# include <ntg/behaviour.hh>
# include <ntg/global_ops.hh>
# include <ntg/optraits_scalar.hh>
# include <ntg/rec_value.hh>
# include <ntg/typetraits.hh>

// FIXME: optraits_range.hh is included at the end of the file.

namespace oln
{

  //
  // Typetraits
  //
  //////////////

  template <class T, class interval, class behaviour>
  struct typetraits<range<T, interval, behaviour> >
  {
    typedef range<T, interval, behaviour> self;
    typedef optraits<self> optraits;
    typedef typename behaviour::get<self> behaviour_type;

    typedef typename typetraits<T>::base_type		base_type;
    typedef T						storage_type;
    typedef typename typetraits<T>::signed_type		signed_type;
    typedef typename typetraits<T>::unsigned_type	unsigned_type;
    typedef typename typetraits<T>::cumul_type		cumul_type;
    typedef typename typetraits<T>::largest_type   	largest_type;
    typedef typename typetraits<T>::signed_largest_type signed_largest_type;
    typedef typename typetraits<T>::signed_cumul_type	signed_cumul_type;
    typedef 
    typename typetraits<T>::unsigned_largest_type	unsigned_largest_type;
    typedef typename typetraits<T>::unsigned_cumul_type	unsigned_cumul_type;
    typedef typename typetraits<T>::integer_type	integer_type;

    // internal type used for binary operations traits
    typedef typename typetraits<T>::base_type op_traits;
  };

  namespace type_definitions
  {

    //
    //  Class range<DecoratedType, Min, Max, Behaviour>
    //
    ////////////////////////////////////////////////////

    template <class T, class interval, class behaviour>
    class range : public rec_scalar<range<T, interval, behaviour> >
    {
    public:
      typedef range<T, interval, behaviour> self;

    private:
      // shortcuts
      typedef typename typetraits<self>::optraits optraits_type;
      typedef typename typetraits<self>::base_type base_type;
      typedef typename typetraits<base_type>::storage_type base_storage_type;

    public:
      range () { _value = 0; }

      template <class U>
      range (const U& u)
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

    template<class T, class interval, class behaviour>
    inline std::ostream&
    operator<<(std::ostream& stream, 
	       const oln::range<T, interval, behaviour>& rhs)
    {
      // Cast useful for range<unsigned char, ...>
      stream << (typename typetraits<T>::largest_type)(rhs.value());
      return stream;
    }

  } // type_definitions
} // end of namespace oln

// FIXME: find another solution if we want self contained range.hh.
# include <ntg/optraits_range.hh>

#endif // ndef OLENA_VALUE_RANGE_HH
