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

#ifndef NTG_REAL_RANGE_HH
# define NTG_REAL_RANGE_HH

# include <mlc/is_a.hh>

# include <ntg/basics.hh>
# include <ntg/core/internal/macros.hh>
# include <ntg/real/optraits_scalar.hh>

# include <string>
# include <sstream>

namespace ntg {

  namespace internal {

    /*------------------.
    | typetraits<range> |
    `------------------*/

    template <class T, class interval, class behavior>
    struct typetraits<range<T, interval, behavior> >
    {
      typedef range<T, interval, behavior>	self;
      typedef ntgi_abstract_type(T)		abstract_type;
      typedef self				ntg_type;
      typedef optraits<self>			optraits_type;
      typedef typename behavior::template get<self>	behavior_type;

      typedef ntgi_base_type(T)			base_type;
      typedef T					storage_type;
      typedef ntgi_signed_type(T)		signed_type;
      typedef ntgi_unsigned_type(T)		unsigned_type;
      typedef ntgi_cumul_type(T)		cumul_type;
      typedef ntgi_largest_type(T)		largest_type;
      typedef ntgi_signed_largest_type(T)	signed_largest_type;
      typedef ntgi_signed_cumul_type(T)		signed_cumul_type;
      typedef ntgi_unsigned_largest_type(T)	unsigned_largest_type;
      typedef ntgi_unsigned_cumul_type(T)	unsigned_cumul_type;
      typedef ntgi_integer_type(T)		integer_type;
    };

  } // end of internal.

  /*-----------------------------.
  | range<T, Interval, Behavior> |
  `-----------------------------*/

  //! Restrict the interval of a type.
  /*
    Range is a decorator. Thanks to the build_value_type helper in
    typetraits, it can insert into a precise place of the hierarchy,
    depending on T.
  */
  template <class T, class interval, class behavior>
  class range : 
    public type_traits<T>::template build_value_type<range<T, interval, behavior> >::ret
  {
  public:
    typedef range<T, interval, behavior> self;

  private:
    typedef ntgi_optraits_type(self)	optraits_type;
    typedef ntg_base_type(self)		base_type;
    typedef ntg_storage_type(base_type)	base_storage_type;

  public:
    range () { this->val_ = 0; }

    template <class U>
    range (const U& u)
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

    template <class U>
    operator U() const { return this->val_; }
  };

  template<class T, class interval, class behavior>
  inline std::ostream&
  operator<<(std::ostream& stream, const range<T, interval, behavior>& rhs)
  {
    // Cast useful for range<unsigned char, ...>
    stream << (ntg_largest_type(T))(rhs.val());
    return stream;
  }

  namespace internal {

    /*----------------.
    | optraits<range> |
    `----------------*/

    template<class T, class interval, class behavior>
    struct optraits<range<T, interval, behavior> > : public optraits<T>
    {
    public:
      typedef range<T, interval, behavior> self;

    private:
      typedef typename typetraits<self>::storage_type	storage_type_;
      typedef typename behavior::template get<self>		behavior_type_;
      typedef typename interval::storage_type		interval_type_;

    public:
      template <class P>
      static storage_type_ check(const P& rhs)
      { return behavior_type_::check(rhs); }

      static interval_type_ min()
      { return interval::min(); }
    
      static interval_type_ max()
      { return interval::max(); }

      static interval_type_ inf()
      { return interval::inf(); }
    
      static interval_type_ sup()
      { return interval::sup(); }
    
      static std::string
      name() 
      {
	std::ostringstream out;
	out << "range<" << optraits<T>::name() << ", " << interval::name() 
	    << ", " << behavior::name() << ">"<< std::ends;
	return out.str();
      }
    };

    /*----------------.
    | operator traits |
    `----------------*/

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class B, class U>
    struct operator_traits<Op, range<T, I, B>, U> 
      : public deduce_from_traits<Op, T, U>::deduced_traits
    {}; 

    // Inherit operator traits from the base type.
    template <class Op, class T, class I, class B, class U>
    struct operator_traits<Op, U, range<T, I, B> >
      : public deduce_from_traits<Op, U, T>::deduced_traits
    {};

    // Inherit operator traits from the base type.
    template <class Op, 
	      class T1, class I1, class B1, 
	      class T2, class I2, class B2>
    struct operator_traits<Op, range<T1, I1, B1>, range<T2, I2, B2> >
      : public deduce_from_traits<Op, T1, T2>::deduced_traits
    {};
 
  } // end of internal.
  
} // end of ntg.

#endif // !NTG_REAL_RANGE_HH
