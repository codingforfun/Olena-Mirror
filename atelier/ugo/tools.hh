#ifndef   	TOOLS_HH_
# define   	TOOLS_HH_

namespace ugo
{
  template <typename T>
  struct singleton
  {
      T value;

      singleton () {}
      singleton (T value) : value(value) {}
  };

  //--vector-------

  template <unsigned n, typename T>
  struct vec
  {
      T comp[n];
      T  operator[] (unsigned i) const { assertion(i < n); return comp[i]; }
      T& operator[] (unsigned i)       { assertion(i < n); return comp[i]; }
  };

  template <unsigned n, typename T>
  std::ostream& operator<<(std::ostream& ostr, const vec<n, T>& vect)
  {
    for (unsigned i = 0; i < n; ++i)
      std::cout << vect.comp[i] << ' ';
    std::cout << std::endl;
    return ostr;
  }
}
#endif	    /* !TOOLS_HH_ */
