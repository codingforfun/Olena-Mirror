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

#ifndef OLENA_VALUE_VEC_HH
# define OLENA_VALUE_VEC_HH

# include <oln/types/rec_value.hh>
# include <oln/types/typetraits.hh>
# include <oln/core/type.hh>
# include <oln/meta/array.hh>

namespace oln {

  template <unsigned N, class T, class Self>
  struct typetraits<vec<N, T, Self> >
  {
    typedef vec<N, T, Self> self;
    typedef optraits<self> optraits;

    typedef typename typetraits<T>::base_type  base_type[N];
    typedef T storage_type[N];
    typedef typename typetraits<T>::cumul_type cumul_type[N];

    // FIXME: useful?
    typedef self op_traits;
  };

  namespace type_definitions {

    template <unsigned N, class T, class Self>
    class vec : 
      public rec_vector<typename type::select_self<vec<N, T, type::bottom>, Self>::ret>
    {
    public :

      vec()
      {
	_fill (optraits<T>::zero());
      }

      /* A vector can be built from a 1xM array.  */
      template<int ncols, class T2>
      vec(const oln::meta::array2d< oln::meta::array2d_info<1, ncols>, T2>&
	  arr)
      {
	meta::eq< ncols, N >::ensure();
	for (unsigned i = 0; i < N; ++i)
	  _value[i] = arr[i];
      }

      template<class U, class Self2>
      vec(const vec<N, U, Self2>& v)
      {
	for (unsigned i = 0; i < N; ++i)
	  _value[i] = v[i];
      }

      template<class U, class Self2>
      vec<N, T>& operator=(const vec<N, U, Self2>& v)
      {
	for (unsigned i = 0; i < N; ++i)
	  _value[i] = v[i];
	return *this;
      }

      // accessor
      T &     operator[](unsigned i) 	   { return _value[i]; }
      const T operator[](unsigned i) const { return _value[i]; }

      unsigned size() const { return N; }

      static const vec<N,T> zero() { return vec(); }
      // There is no unit() for vec<>.
      ~vec() {}

    protected:
      vec& _fill(T t)
      {
	for (unsigned i = 0; i < N; ++i)
	  _value[i] = t;
	return *this;
      }
    };

  } // type_definitions

} // end of oln



template<unsigned N,class T> inline
std::ostream& operator<<(std::ostream& ostr, const oln::vec<N,T>& rhs)
{
  ostr << "[";
  for (unsigned i = 0; i < N; ++i)
    ostr << rhs[i] << ((i < N-1) ? "," : "]");
  return ostr;
}


#endif // ! OLENA_VALUE_VEC_HH
