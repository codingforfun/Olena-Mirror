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

#ifndef NTG_REAL_CYCLE_HH
# define NTG_REAL_CYCLE_HH

# include <ntg/basics.hh>
# include <ntg/core/internal/macros.hh>
# include <ntg/core/interval.hh>

# include <mlc/is_a.hh>

# include <string>
# include <sstream>

namespace ntg {

  namespace internal {

    /*------------------.
    | typetraits<cycle> |
    `------------------*/

    template <class T, class interval>
    struct typetraits<cycle<T, interval> >
    {
      typedef cycle<T, interval>			self;
      typedef typename typetraits<T>::abstract_type	abstract_type;
      typedef self					ntg_type;
      typedef optraits<self>				optraits_type;
      typedef cycle_behavior::get<self>			behavior_type;

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

  /*-------------------.
  | cycle<T, interval> |
  `-------------------*/

  template <class T,
	    class interval>
  class cycle : 
    public type_traits<T>::template build_value_type<cycle<T, interval> >::ret
  {
  public:
    typedef cycle<T, interval> self;

  private:
    typedef ntgi_optraits_type(self)		optraits_type;
    typedef ntg_base_type(self)			base_type;
    typedef ntgi_storage_type(base_type)	base_storage_type;

  public:
    cycle () { this->val_ = 0; }

    template <class U>
    cycle (const U& u)
    {
      ntg_is_a(U, real)::ensure();
      this->val_ = optraits_type::check(u);
    }
    template <class U>
    self&
    operator=(const U& u)
    {
      this->val_ = optraits_type::check(u);
      return *this;
    }

    operator base_storage_type() const { return this->val_; }
  };

  template<class T, class interval>
  inline std::ostream&
  operator<<(std::ostream& stream, const cycle<T, interval>& rhs)
  {
    // FIXME: cast needed for cycle<unsigned char, ...>.
    stream << (ntg_largest_type(T))(rhs.val());
    return stream;
  }

  namespace internal {

    /*----------------.
    | optraits<cycle> |
    `----------------*/

    template<class T,
	     class interval>
    struct optraits<cycle<T, interval> > : public optraits<T>
    {
    public:
      typedef cycle<T, interval> self;
    
    private:
      typedef typename typetraits<self>::storage_type	storage_type_;
      typedef typename interval::storage_type		interval_type_;
      typedef typename typetraits<self>::behavior_type	behavior_type_;

    public:
      static interval_type_ min()
      { return interval::min(); }

      static interval_type_ max()
      { return interval::max(); }

      static interval_type_ inf()
      { return interval::inf(); }

      static interval_type_ sup()
      { return interval::sup(); }

      template <class P>
      static storage_type_
      check(const P& rhs)
      { return behavior_type_::check(rhs); }

      static std::string
      name()
      {
	std::ostringstream out;
	out << "cycle<" << optraits<T>::name() << ", " 
	    << interval::name() << ">"<< std::ends;
	return out.str();
      }
    };

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class U>
    struct operator_traits<Op, cycle<T, I>, U> 
      : public deduce_from_traits<Op, T, U>::deduced_traits
    {}; 

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class U>
    struct operator_traits<Op, U, cycle<T, I> >
      : public deduce_from_traits<Op, U, T>::deduced_traits
    {};

    // Inherit operator traits from the base type.
    template <class Op, class T1, class I1, class T2, class I2>
    struct operator_traits<Op, cycle<T1, I1>, cycle<T2, I2> >
      : public deduce_from_traits<Op, T1, T2>::deduced_traits
    {};

  } // end of internal.

} // end of ntg.

#endif // !NTG_REAL_CYCLE_HH
