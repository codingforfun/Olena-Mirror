
#ifndef UTILS_RANGE_HH
# define UTILS_RANGE_HH

# include <mln/value/int_u8.hh> // To get whatever we need for a value type!


namespace mln
{


  // Fwd decl.
  namespace utils {
    template <typename T> struct range;
  }


  namespace trait {

    template <template <class> class Op, typename T>
    struct set_precise_unary_< Op, utils::range<T> >
    {
      typedef bool ret;
    };

    template <template <class, class> class Op, typename T, typename V>
    struct set_precise_binary_< Op, utils::range<T>, V >
    {
      typedef bool ret;
    };

  } // mln::traits



  namespace utils
  {


    // Range type.


    template <typename T>
    struct range
    {
      enum {
	nbits_  = mln_nbits(T),
	max = mlc_pow_int(2, nbits_) - 1
      };

      typedef value::Integer<void> category;
      typedef T value;

      T lower, upper; 

      range()
      {
      }

      range(const T& lower, const T& upper)
	: lower(lower), upper(upper)
      {
	mln_invariant(upper >= lower);
      }

      void set(const T& v)
      {
	lower = v;
	upper = v;
      }

      unsigned len() const
      {
	mln_invariant(upper >= lower);
	return upper - lower + 1;
      }

      unsigned variation() const
      {
	mln_invariant(upper >= lower);
	return upper - lower;
      }

      void alphaise(unsigned alpha)
      {
	mln_invariant(upper >= lower);
	int l = int(lower) - alpha / 2, u = int(upper) + alpha / 2;
	lower = l < 0 ? 0 : l;
	upper = u > max ? max : u;
	mln_invariant(upper >= lower);
      }
    };


    template <typename T>
    inline
    range<T>
    make_range(const T& t1, const T& t2)
    {
      if (t1 < t2)
	return range<T>(t1, t2);
      else
	return range<T>(t2, t1);
    }


//     template <typename T> inline
//     const T& lower(const utils::range<T>& r) { return r.lower; }

//     template <typename T> inline
//     const T& lower(const T& t) { return t; }
    
//     template <typename T> inline
//     const T& upper(const utils::range<T>& r) { return r.upper; }

//     template <typename T> inline
//     const T& upper(const T& t) { return t; }


    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const range<T>& r)
    {
      return ostr << '[' << r.lower << ',' << r.upper << ']';
    }


    // set operations:
    //   'or'  is "union", supremum
    //   'and' is "inter", infemum


    template <typename T>
    inline
    range<T> operator || (const range<T>& r1, const range<T>& r2)
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
		    ((r1 || r2).len() < r1.len() + r2.len()));
      return std::max(r1.lower, r2.lower) <= std::min(r1.upper, r2.upper);
    }

    template <typename T>
    inline
    range<T> operator && (const range<T>& r1, const range<T>& r2)
    {
      T lower = std::max(r1.lower, r2.lower),
	upper = std::min(r1.upper, r2.upper);
      mln_precondition(lower <= upper); // no possible empty set!!!
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



  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_RANGE_HH
