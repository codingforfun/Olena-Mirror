// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_CYCLE_HH
# define NTG_CYCLE_HH

# include <ntg/config/system.hh>

# include <mlc/is_a.hh>

# include <ntg/basics.hh>
# include <ntg/core/interval.hh>

# include <string>
# include <sstream>

namespace ntg {

  namespace internal {

    //
    //  Typetraits
    //
    ///////////////

    template <class T, class interval>
    struct typetraits<cycle<T, interval> >
    {
      typedef cycle<T, interval> self;
      typedef typename typetraits<T>::abstract_type abstract_type;
      typedef self ntg_type;
      typedef optraits<self> optraits;
      typedef cycle_behaviour::get<self> behaviour_type;

      typedef typename typetraits<T>::base_type		base_type;
      typedef T						storage_type;
      typedef typename typetraits<T>::signed_type	signed_type;
      typedef typename typetraits<T>::unsigned_type	unsigned_type;
      typedef self					cumul_type;
      typedef self					largest_type;
      typedef self					signed_largest_type;
      typedef self					signed_cumul_type;
      typedef self					unsigned_largest_type;
      typedef self					unsigned_cumul_type;
      typedef typename typetraits<T>::integer_type	integer_type;
    };

  } // end of internal.

  //
  //  Class cycle<DecoratedType, class interval>
  //
  //  The interval has 0
  //
  ////////////////////////////////////////////////////

  template <class T,
	    class interval>
  class cycle : public real_value<cycle<T, interval> >
  {
  public:
    typedef cycle<T, interval> self;

  private:
    // shortcuts
    typedef typename internal::typetraits<self>::optraits optraits_type;
    typedef ntg_base_type(self) base_type;
    //    typedef ntg_storage_type(base_type) base_storage_type;
    typedef typename type_traits<base_type>::storage_type base_storage_type;

  public:
    cycle () { val_ = 0; }

    template <class U>
    cycle (const U& u)
    {
      ntg_is_a(U, real)::ensure();
      val_ = optraits_type::check(u);
    }
    template <class U>
    self& operator=(const U& u)
    {
      val_ = optraits_type::check(u);
      return *this;
    }

    // cast
    operator base_storage_type() const { return val_; }
  };

  template<class T, class interval>
  inline std::ostream&
  operator<<(std::ostream& stream, const cycle<T, interval>& rhs)
  {
    // Cast useful for cycle<unsigned char, ...>
    stream << (ntg_largest_type(T))(rhs.val());
    return stream;
  }

  namespace internal {

    template<class T,
	     class interval>
    struct optraits<cycle<T, interval> > : public optraits<T>
    {
    public:
      typedef cycle<T, interval> self;
    
    private:
      typedef typename typetraits<self>::storage_type storage_type_;
      typedef typename interval::storage_type interval_type_;
      typedef typename typetraits<self>::behaviour_type behaviour_type_;

    public:
      //
      //  Properties
      //
      ////

      static interval_type_ min()
      { return interval::min(); }

      static interval_type_ max()
      { return interval::max(); }

      static interval_type_ inf()
      { return interval::inf(); }

      static interval_type_ sup()
      { return interval::sup(); }


      // behaviour's check

      template <class P>
      static storage_type_ check(const P& rhs)
      { return behaviour_type_::apply(rhs); }

      // debug
      static std::string name() {
	std::ostringstream out;
	out << "cycle<" << optraits<T>::name() << ", " 
	    << interval::name() << ">"<< std::ends;
	return out.str();
      }
    };

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class U>
    struct operator_traits<Op, cycle<T, I>, U> 
      : public operator_traits<Op, T, U>
    {}; 

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class U>
    struct operator_traits<Op, U, cycle<T, I> >
      : public operator_traits<Op, U, T>
    {};

    // Inherit operator traits from the base type.
    template <class Op, class T1, class I1, class T2, class I2>
    struct operator_traits<Op, cycle<T1, I1>, cycle<T2, I2> >
      : public operator_traits<Op, T1, T2>
    {};

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_CYCLE_HH
