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

#ifndef OLENA_TOPO_DMAP_HXX
# define OLENA_TOPO_DMAP_HXX

# include <oln/arith/ops.hh>
# include <oln/types/optraits_builtins_float.hh>
# include <oln/meta/array2d.hh>

namespace oln {

  namespace topo {

    template <class T>
    chamfer<T>::chamfer(const w_window2d<T>& fwd,
			const w_window2d<T>& bkd,
			float		   coef) :
      fwd(fwd),
      bkd(bkd),
      coef(coef)
    {
    }

    template <class T>
    coord chamfer<T>::delta() const
    {
      coord d = fwd.delta();
      invariant(bkd.delta() == d);
      return d;
    }

    // mk_chamfer...
    // FIXME: this is highly not thread safe !
    template<int d10, int d11> inline
    const chamfer<int>& mk_chamfer_3x3(float coef = 1.f)
    {
      static const w_window2d<int> w_win_fwd = ( meta::ints_2d =
					       d11, d10, d11, lbrk,
					       d10, x(),   0, end );
      static const w_window2d<int> w_win_bkd = ( meta::ints_2d =
					       0, x(),  d10, lbrk,
					       d11, d10,  d11, end );
      static const chamfer<int> _ch = chamfer<int>(w_win_fwd, w_win_bkd, coef);
      return _ch;
    }

    inline // FIXME: how to define float by parameters?
    const chamfer<float>& mk_chamfer_3x3(float d10, float d11) 
      // FIXME: add (?)  , float coef = 1.f
    {
      static const w_window2d<float> w_win_fwd = ( meta::floats_2d =
						 d11, d10, d11, lbrk,
						 d10, x(), 0.f, end );
      static const w_window2d<float> w_win_bkd = ( meta::floats_2d =
						 0.f, x(),  d10, lbrk,
						 d11, d10,  d11, end );
      static const chamfer<float> _ch = 
	chamfer<float>(w_win_fwd, w_win_bkd, 1.f);
      return _ch;
    }

    template<int d10, int d11, int d21> inline
    const chamfer<int>& mk_chamfer_5x5(float coef = 1.f)
    {
      static const w_window2d<int> w_win_fwd = ( meta::ints_2d =
					       0, d21,   0, d21,   0, lbrk,
					       d21, d11, d10, d11, d21,
					       0, d10, x(),   0,   0, end );
      static const w_window2d<int> w_win_bkd = ( meta::ints_2d =
					       0,   0, x(), d10,   0, lbrk,
					       d21, d11, d10, d11, d21,
					       0, d21,   0, d21,   0, end );
      static const chamfer<int> _ch = chamfer<int>(w_win_fwd, w_win_bkd, coef);
      return _ch;
    }

    inline
    const chamfer<float>& mk_chamfer_5x5(float d10, float d11, float d21)
    {
      const float O = 0.f;
      static const w_window2d<float> w_win_fwd = ( meta::floats_2d =
						 O, d21,   O, d21,   O, lbrk,
						 d21, d11, d10, d11, d21,
						 O, d10, x(),   O,   O, end );
      static const w_window2d<float> w_win_bkd = ( meta::floats_2d =
						 O,   O, x(), d10,   O, lbrk,
						 d21, d11, d10, d11, d21,
						 O, d21,   O, d21,   O, end );
      static const chamfer<float> _ch = 
	chamfer<float>(w_win_fwd, w_win_bkd, 1.f);
      return _ch;
    }

    // REF: B.J.H. Verwer, Local distances for distance transformations
    // in two and three dimensions, Pattern Recognition Letters 12 (1991) 671-682

    // unbiased minimal mean square error for integer local distances (Table 5)
#define CHAMFER2(Name, I, J, D, E) \
    inline const chamfer<int>& Name##_##I##_##J##()		\
     { 								\
       static const chamfer<int> tmp =                          \
          mk_chamfer_##D##x##D##< I, J>(E);   	                \
       return tmp;						\
     }

#define CHAMFER3(Name, I, J, K, D, E)				\
    inline const chamfer<int>& Name##_##I##_##J##_##K##()	\
    {								\
      static const chamfer<int> tmp =				\
         mk_chamfer_##D##x##D##< I, J, K>(E);			\
      return tmp;						\
    }

    CHAMFER2(chamfer, 1,   1,  3, 0.9003);
    CHAMFER2(chamfer, 1,   2,  3, 1.2732);
    CHAMFER2(chamfer,  2,   3,  3, 2.1736);
    CHAMFER2(chamfer,  5,   7,  3, 5.2474);
    CHAMFER2(chamfer, 12,  17,  3, 12.6684);

    inline const chamfer<int>& chessboard()    
    { 
      return chamfer_1_1(); 
    }

    inline const chamfer<int>& cityblock()     
    { 
      return chamfer_1_2(); 
    }

    CHAMFER3(chamfer, 4,  6, 9, 5, 4.1203);
    CHAMFER3(chamfer, 5,  7, 11, 5, 5.0206);
    CHAMFER3(chamfer, 9,  13, 20, 5, 9.1409);
    CHAMFER3(chamfer, 16, 23, 36, 5, 16.3351);

    inline const chamfer<float>& best_set_3x3() 
    { return mk_chamfer_3x3(0.9481, 1.3408); }
    inline const chamfer<float>& best_set_5x5() 
    { return mk_chamfer_5x5(0.9801, 1.4060, 2.2044); }


    // maximum absolute error for integer local distances (Table 2)
    CHAMFER2(mchamfer, 1, 1, 3, 0.8536);
    CHAMFER2(mchamfer, 1, 2, 3, 1.2071);
    CHAMFER2(mchamfer, 2, 3, 3, 2.1180);
    CHAMFER2(mchamfer, 5, 7, 3, 5.1675);
    CHAMFER2(mchamfer, 12, 17, 3, 12.5000);

    inline const chamfer<int>& mchessboard()    { return mchamfer_1_1(); }
    inline const chamfer<int>& mcityblock()     { return mchamfer_1_2(); }

    CHAMFER3(mchamfer, 4,  6,  9, 5, 4.1213);
    CHAMFER3(mchamfer, 5,  7, 11, 5, 5.0092);
    CHAMFER3(mchamfer, 9, 13, 20, 5, 9.0819);
    CHAMFER3(mchamfer, 17, 24, 38, 5, 17.2174);

    inline const chamfer<float>& mbest_set_3x3() {
      const float coef = 1.0412;
      return mk_chamfer_3x3(1/coef, sqrt(2.f)/coef);
    }
    inline const chamfer<float>& mbest_set_5x5() {
      const float coef = 1.0137;
      return mk_chamfer_5x5(1/coef, sqrt(2.f)/coef, sqrt(5.f)/coef);
    }

#undef CHAMFER2
#undef CHAMFER3


    template <class T, class T2>
    dmap<T, T2>::dmap(const image2d_size&  size,
		      const chamfer<T2>& ch) :
      _imap(size),
      _ch(ch)
    {
      // FIXME: if T is float then precondition(ch.coef == 1.f)
    }

    template <class T, class T2>
    template <class V>
    void dmap<T, T2>::compute(const image2d<V>&   input, 
			      float		  infty = 0.f)
    {
      image2d<point> dummy;
      compute(input, dummy, infty);
    }

    template <class T, class T2>
    template <class V>
    void dmap<T, T2>::compute(const image2d<V>&   input, 
			      image2d<point>&       nearest_point_map,
			      float			infty = 0.f)
    {
      precondition(input.size() == _imap.size());
      if (infty == 0.f)
	{
	  _inFty = 1e4;
	  _infTy = 1e4; // FIXME: !!!
	}
      else
	{
	  _inFty = infty;
	  _infTy = T(infty); // FIXME: use ceil if T is integer!
	}

      // init
      {
	image2d<V>::iter p(input);
	for (p = begin; p != end; ++p)
	  if (input[p] != optraits<T>::zero())
	    {
	      _imap[p] = T(0);
	      nearest_point_map[p] = p;
	    }
	  else
	    _imap[p] = _infTy;
	border::adapt_copy(_imap, _ch.delta()); // FIXME: this is geodesic only!
      }

      // fwd
      {
	image2d<V>::fwd_iter p(input);
	for (p = begin; p != end; ++p)
	  {
	    if (_imap[p] == T(0))
	      continue;
	    T min = _imap[p];
	    for (unsigned i = 0; i < _ch.fwd.card(); ++i)
	      {
		point q = p + _ch.fwd.dp(i);
		if (_imap[q] == _infTy) // FIXME: || _imap[q] >= min
		  continue;
		if (_imap[q] + _ch.fwd.w(i) < min)
		  {
		    nearest_point_map[p] = nearest_point_map[q];		    
		    min = _imap[q] + _ch.fwd.w(i);
		  }
	      }
	    _imap[p] = min;
	  }
      }

      // bkd
      {
	image2d<V>::bkd_iter p(input);
	for_all(p)
	  {
	    if (_imap[p] == T(0))
	      continue;
	    T min = _imap[p];
	    for (unsigned i = 0; i < _ch.bkd.card(); ++i)
	      {
		point q = p + _ch.bkd.dp(i);
		if (_imap[q] == _infTy) // FIXME: || _imap[q] >= min
		  continue;
		if (_imap[q] + _ch.bkd.w(i) < min)
		  {
		    nearest_point_map[p] = nearest_point_map[q];
		    min = _imap[q] + _ch.bkd.w(i);
		  }
	      }
	    _imap[p] = min;
	  }
      }
    }

    template <class T, class T2>
    const image2d<T>& dmap<T, T2>::imap() const 
    { 
      return _imap; 
    }

    template <class T, class T2>
    image2d<float> dmap<T, T2>::to_image() const
    {
      // FIXME: if T is float, call invariant(_ch.coef == 1.f);
      // and then return imap();
      // otherwise: 	return arith::div(_imap, _ch.coef);
      image2d<float> output(_imap.size());
      typename image2d<float>::iter p(_imap);
      for_all(p)
	output[p] = (_imap[p] == _infTy ?
		     _inFty :
		     _imap[p] / _ch.coef);
      return output;
    }

    template <class T, class T2>
    const T& dmap<T, T2>::operator[](const point& p) const
    {
      return _imap[p] / _ch.coef;
    }

    template <class T, class T2>
    const T& dmap<T, T2>::operator()(coord row, coord col) const
    {
      return _imap(row, col) / _ch.coef;
    }

    inline float euclidian_dist2(const point2d& p1, const point2d& p2)
    {
      float dr = p1.row() - p2.row();
      float dc = p1.col() - p2.col();
      return dr * dr + dc * dc;
    }

    image2d<float> exact_dmap(const image2d<bin>& input)
    {
      image2d<float> output(input.size());
      image2d<float>::fwd_iter p(input);
      for_all(p)
	{
	  if (input[p] == true)
	    {
	      output[p] = 0.f;
	      continue;
	    }
	  image2d<float>::fwd_iter q(input);
	  bool found = false;
	  float d = 0.f;
	  for_all(q)
	    {
	      if (input[q] == false || q == p)
		continue;
	      if (! found)
		{
		  d = euclidian_dist2(p, q);
		  found = true;
		  continue;
		}
	      d = std::min(euclidian_dist2(p, q), d);
	    }
	  output[p] = sqrt(d);
	}
      return output;
    }

  } // end of topo

} // end of oln


#endif // OLENA_TOPO_DMAP_HXX
