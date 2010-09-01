#ifndef RANGE_HH
# define RANGE_HH


#include <mln/value/int_u8.hh>


namespace mln
{


  // Fwd decl.
  template <typename T> struct range;


  namespace trait {

    template <template <class> class Op, typename T>
    struct set_precise_unary_< Op, range<T> >
    {
      typedef bool ret;
    };

    template <template <class, class> class Op, typename T, typename V>
    struct set_precise_binary_< Op, range<T>, V >
    {
      typedef bool ret;
    };

  } // mln::traits



  // Range type.

  template <typename T>
  struct range
  {
    typedef value::Integer<void> category;
    typedef T value;

    T lower, upper; 

    range()
    {
    }

    range(T lower, T upper)
      : lower(lower), upper(upper)
    {
      mln_invariant(upper >= lower);
    }

    unsigned len() const
    {
      mln_invariant(upper >= lower);
      return upper - lower + 1;
    }
  };


  template <typename T>
  inline
  std::ostream& operator<<(std::ostream& ostr, const range<T>& r)
  {
    return ostr << '[' << r.lower << ',' << r.upper << ']';
  }


  // set operations

  template <typename T>
  inline
  range<T> uni(const range<T>& r1, const range<T>& r2)
  {
    range<T> r(std::min(r1.lower, r2.lower), std::max(r1.upper, r2.upper));
    return r;
  }

  template <typename T>
  inline
  bool intersects(const range<T>& r1, const range<T>& r2)
  {
    mln_invariant((std::max(r1.lower, r2.lower) <= std::min(r1.upper, r2.upper))
		  ==
		  (uni(r1, r2).len() < r1.len() + r2.len()));
    return std::max(r1.lower, r2.lower) <= std::min(r1.upper, r2.upper);
  }

  template <typename T>
  inline
  range<T> inter(const range<T>& r1, const range<T>& r2)
  {
    T lower = std::max(r1.lower, r2.lower),
      upper = std::min(r1.upper, r2.upper);
    mln_precondition(lower <= upper);
    range<T> r(lower, upper);
    return r;
  }



  // r op v

  template <typename T, typename V>
  inline
  bool operator==(const range<T>& r, const V& v)
  {
    return r.lower <= v && v <= r.upper;
  }

  template <typename T, typename V>
  inline
  bool operator!=(const range<T>& r, const V& v)
  {
    return v < r.lower || v > r.upper;
  }

  template <typename T, typename V>
  inline
  bool operator>=(const range<T>& r, const V& v)
  {
    // a value in the range is greater or equal than v
    return r.upper >= v;
  }

  template <typename T, typename V>
  inline
  bool operator>(const range<T>& r, const V& v)
  {
    return r.upper > v;
  }

  template <typename T, typename V>
  inline
  bool operator<=(const range<T>& r, const V& v)
  {
    // a value in the range is lower or equal than v
    return r.lower <= v;
  }

  template <typename T, typename V>
  inline
  bool operator<(const range<T>& r, const V& v)
  {
    return r.lower < v;
  }


  // v op r

  template <typename V, typename T>
  inline
  bool operator==(const V& v, const range<T>& r)
  {
    return r == v;
  }

  template <typename V, typename T>
  inline
  bool operator!=(const V& v, const range<T>& r)
  {
    return r != v;
  }

  template <typename V, typename T>
  inline
  bool operator<(const V& v, const range<T>& r)
  {
    return r > v;
  }

  template <typename V, typename T>
  inline
  bool operator<=(const V& v, const range<T>& r)
  {
    return r >= v;
  }

  template <typename V, typename T>
  inline
  bool operator>(const V& v, const range<T>& r)
  {
    return r < v;
  }

  template <typename V, typename T>
  inline
  bool operator>=(const V& v, const range<T>& r)
  {
    return r <= v;
  }



} // mln


#endif // ! RANGE_HH
