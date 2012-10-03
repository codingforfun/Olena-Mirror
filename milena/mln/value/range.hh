// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_VALUE_RANGE_HH
# define MLN_VALUE_RANGE_HH

/// \file
///
/// Define a range value.

# include <cstdlib>
# include <iostream>
# include <mln/value/next.hh>

namespace mln
{

  namespace value
  {

    template <typename T>
    class range
    {
    public:
      range();

      template <typename T_>
      range(T_ value);

      range(T lower, T upper);

      range& operator=(const range& rhs);

      template <typename T_>
      range& operator=(T_ value);

      bool has(const T& v) const;

      T length() const;

      unsigned nelements() const;

      bool is_degenerated() const;

      const T& lower() const;
      const T& upper() const;

      const T& first() const;
      const T& last() const;

      operator T() const;

    private:
      T lower_;
      T upper_;
    };

//  comparison

    template <typename T>
    bool
    operator==(const range<T>& lhs, const range<T>& rhs);

    template <typename T>
    bool
    operator!=(const range<T>& lhs, const range<T>& rhs);

// deactivation of ordering related operators

    template <typename T>
    void operator<(const range<T>&, const range<T>&);

    template <typename T>
    void operator<=(const range<T>&, const range<T>&);

    template <typename T>
    void operator>(const range<T>&, const range<T>&);

    template <typename T>
    void operator>=(const range<T>&, const range<T>&);

//  set ops

    template <typename T>
    bool
    are_adjacent(const range<T>& r1, const range<T>& r2);

    template <typename T>
    bool
    do_intersect(const range<T>& r1, const range<T>& r2);

    template <typename T>
    range<T>
    inter(const range<T>& r1, const range<T>& r2);

//  min / max

    template <typename T>
    range<T>
    min(const range<T>& r1, const range<T>& r2);

    template <typename T>
    range<T>
    max(const range<T>& r1, const range<T>& r2);

// mean

    template <typename T>
    range<T>
    mean(const range<T>& r1, const range<T>& r2);

//  span

    template <typename T>
    range<T>
    span(const range<T>& r1, const range<T>& r2);

//  op<<

    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const range<T>& i);

  } // end of namespace mln::value

} // end of namespace mln


// for sorting purpose

namespace std
{

  template <typename T>
  struct less< mln::value::range<T> >
  {
    bool operator()(const mln::value::range<T>& l,
		    const mln::value::range<T>& r) const;
  };

} // std



# ifndef MLN_INCLUDE_ONLY


namespace mln
{

  namespace value
  {


    template <typename T>
    range<T>::range()
    {
    }

    template <typename T>
    template <typename T_>
    range<T>::range(T_ value)
    {
      lower_ = upper_ = value;
    }

    template <typename T>
    range<T>::range(T lower, T upper)
    {
      if (upper < lower)
	std::abort();
      lower_ = lower;
      upper_ = upper;
    }

    template <typename T>
    range<T>&
    range<T>::operator=(const range& rhs)
    {
      lower_ = rhs.lower_;
      upper_ = rhs.upper_;
      return *this;
    }

    template <typename T>
    template <typename T_>
    range<T>&
    range<T>::operator=(T_ value)
    {
      lower_ = upper_ = value;
      return *this;
    }

    template <typename T>
    bool
    range<T>::has(const T& v) const
    {
      return lower_ <= v && v <= upper_;
    }

    template <typename T>
    T
    range<T>::length() const
    {
      return upper_ - lower_;
    }

    template <typename T>
    unsigned
    range<T>::nelements() const
    {
      unsigned n = 1;
      T v = lower_;
      for (; v != upper_; v = value::succ(v))
	n += 1;

      return n;
    }

    template <typename T>
    bool
    range<T>::is_degenerated() const
    {
      return upper_ == lower_;
    }

    template <typename T>
    const T&
    range<T>::lower() const
    {
      return lower_;
    }

    template <typename T>
    const T&
    range<T>::upper() const
    {
      return upper_;
    }

    template <typename T>
    const T&
    range<T>::first() const
    {
      return lower_;
    }

    template <typename T>
    const T&
    range<T>::last() const
    {
      return upper_;
    }

    template <typename T>
    range<T>::operator T() const
    {
      if (! is_degenerated())
	std::abort();
      return upper_;
    }


//  comparison

    template <typename T>
    bool
    operator==(const range<T>& lhs, const range<T>& rhs)
    {
      return lhs.lower() == rhs.lower() && lhs.upper() == rhs.upper();
    }

    template <typename T>
    bool
    operator!=(const range<T>& lhs, const range<T>& rhs)
    {
      return ! (lhs == rhs);
    }


// deactivation of ordering related operators

    template <typename T>
    void operator<(const range<T>&, const range<T>&);

    template <typename T>
    void operator<=(const range<T>&, const range<T>&);

    template <typename T>
    void operator>(const range<T>&, const range<T>&);

    template <typename T>
    void operator>=(const range<T>&, const range<T>&);



//  set ops

    template <typename T>
    bool
    are_adjacent(const range<T>& r1, const range<T>& r2)
    {
      return span(r1, r2).length() == r1.length() + r2.length()
	+ (value::succ(r2.lower()) - r2.lower());
    }

    template <typename T>
    bool
    do_intersect(const range<T>& r1, const range<T>& r2)
    {
      return span(r1, r2).length() <= r1.length() + r2.length();
    }

    template <typename T>
    range<T>
    inter(const range<T>& r1, const range<T>& r2)
    {
      if (! do_intersect(r1, r2))
	std::abort();
      return range<T>(std::max(r1.lower(), r2.lower()),
		      std::min(r1.upper(), r2.upper()));
    }



//  min / max

    template <typename T>
    range<T>
    min(const range<T>& r1, const range<T>& r2)
    {
      return range<T>(std::min(r1.lower(), r2.lower()),
		      std::min(r1.upper(), r2.upper()));
    }

    template <typename T>
    range<T>
    max(const range<T>& r1, const range<T>& r2)
    {
      return range<T>(std::max(r1.lower(), r2.lower()),
		      std::max(r1.upper(), r2.upper()));
    }


// mean

    template <typename T>
    range<T>
    mean(const range<T>& r1, const range<T>& r2)
    {
      return range<T>(mean(r1.lower(), r2.lower()),
		      mean(r1.upper(), r2.upper()));
    }


//  span

    template <typename T>
    range<T>
    span(const range<T>& r1, const range<T>& r2)
    {
      return range<T>(std::min(r1.lower(), r2.lower()),
		      std::max(r1.upper(), r2.upper()));
    }



//  op<<

    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const range<T>& i)
    {
      if (i.is_degenerated())
	return ostr << i.lower();
      else
	return ostr << '[' << i.lower() << ',' << i.upper() << ']';
    }


  } // end of namespace mln::value

} // end of namespace mln


namespace std
{

  template <typename T>
  bool less< mln::value::range<T> >::operator()(const mln::value::range<T>& l,
						const mln::value::range<T>& r) const
  {
    if (! l.is_degenerated() || ! r.is_degenerated())
      std::abort();
    return l.lower() < r.lower();
  }

} // std


# endif // ! MLN_INCLUDE_ONLY

#endif // ndef MLN_VALUE_RANGE_HH
