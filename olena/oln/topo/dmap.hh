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

#ifndef OLENA_TOPO_DMAP_HH
# define OLENA_TOPO_DMAP_HH

# include <math.h>
# include <utility>

# include <oln/basics2d.hh>
# include <oln/core/w_window2d.hh>

namespace oln {

  namespace topo {

    template<class T>
    struct chamfer
    {
      chamfer(const w_window2d<T>& fwd,
	      const w_window2d<T>& bkd,
	      float coef);

      coord delta() const;

      const w_window2d<T> fwd;
      const w_window2d<T> bkd;
      const float coef;
    };

    template<int d10, int d11> inline
    const chamfer<int>& mk_chamfer_3x3(float coef = 1.f);

    inline 
    const chamfer<float>& mk_chamfer_3x3(float d10, float d11);

    template<int d10, int d11, int d21> inline
    const chamfer<int>& mk_chamfer_5x5(float coef = 1.f);

    inline
    const chamfer<float>& mk_chamfer_5x5(float d10, float d11, float d21);

    // REF: B.J.H. Verwer, Local distances for distance transformations
    // in two and three dimensions, Pattern Recognition Letters 12 (1991) 671-682
    inline const chamfer<int>& chamfer_1_1();
    inline const chamfer<int>& chamfer_1_2();
    inline const chamfer<int>& chamfer_2_3();
    inline const chamfer<int>& chamfer_5_7();
    inline const chamfer<int>& chamfer_12_17();
    inline const chamfer<int>& chessboard();
    inline const chamfer<int>& cityblock();
    inline const chamfer<int>& chamfer_4_6_9();
    inline const chamfer<int>& chamfer_5_7_11();
    inline const chamfer<int>& chamfer_9_13_20();
    inline const chamfer<int>& chamfer_16_23_36();
    inline const chamfer<float>& best_set_3x3();
    inline const chamfer<float>& best_set_5x5();

    // maximum absolute error for integer local distances (Table 2)
    inline const chamfer<int>& mchamfer_1_1();
    inline const chamfer<int>& mchamfer_1_2();
    inline const chamfer<int>& mchamfer_2_3();
    inline const chamfer<int>& mchamfer_5_7();
    inline const chamfer<int>& mchamfer_12_17();
    inline const chamfer<int>& mchessboard();
    inline const chamfer<int>& mcityblock();
    inline const chamfer<int>& mchamfer_4_6_9();
    inline const chamfer<int>& mchamfer_5_7_11();
    inline const chamfer<int>& mchamfer_9_13_20();
    inline const chamfer<int>& mchamfer_17_24_38();
    inline const chamfer<float>& mbest_set_3x3();
    inline const chamfer<float>& mbest_set_5x5();

    template<class T, class T2>
    class dmap
    {
    public:
      typedef image2d<ntg::bin>::point_type point_type;

      dmap(const image2d_size& size,
	   const chamfer<T2>& ch);

      template <class V>
      void compute(const image2d<V>& input, float infty = 0.f);

      template <class V>
      void compute(const image2d<V>&   input, 
		   image2d<point2d>&     nearest_point_map, 
		   float                 infty = 0.f);

      const image2d<T>& imap() const;
      image2d<float> to_image() const;

      const T& operator[](const point_type& p) const;

      const T& operator()(coord row, coord col) const;

    private:
      image2d<T>	imap_;
      chamfer<T2>	ch_;
      float		inFty_;
      T		       	infTy_;
    };

    template <class I>
    image2d<float> exact_dmap(const abstract::image<I>& input);

  } // end of topo.

} // end of oln.

# include <oln/topo/dmap.hxx>

#endif // OLENA_TOPO_DMAP_HH
