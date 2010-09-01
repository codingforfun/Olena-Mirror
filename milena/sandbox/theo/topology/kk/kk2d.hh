#ifndef KK2D_HH
# define KK2D_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/value/int_u8.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/extend.hh>

#include <mln/core/var.hh>
#include <mln/data/paste.hh>
#include <mln/data/fill.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/gradient_internal.hh>


namespace mln
{



  template <typename I, typename J>
  void data__paste_values(const Image<I>& input_, Image<J>& output_)
  {
    const I& input = exact(input_);
    J& output = exact(output_);
    
    mln_fwd_piter(I) pi(input.domain());
    mln_fwd_piter(J) po(output.domain());
    for_all_2(pi, po)
      output(po) = input(pi);
  }


  namespace kk2d {
    template <typename T> struct range;
  }

  namespace trait {

    template <template <class> class Op, typename T>
    struct set_precise_unary_< Op, kk2d::range<T> >
    {
      typedef bool ret;
    };

    template <template <class, class> class Op, typename T, typename V>
    struct set_precise_binary_< Op, kk2d::range<T>, V >
    {
      typedef bool ret;
    };
    

//     template <typename Op, typename T>
//     struct set_precise_binary_< Op, kk2d::range<T> >
//     {
//       typedef bool ret;
//     };

  }


  namespace kk2d
  {


    // Faces predicates.


    //      0 1 2 3 4 5 6
    //    ---------------
    // 0 |  2 1 1 2 1 1 2
    // 1 |  1 0 0 1 0 0 1
    // 2 |  1 0 0 1 0 0 1
    // 3 |  2 1 1 2 1 1 2


    typedef fun::C<bool (*)(const mln::point2d&)> predicate_t;

    inline
    bool is_2_face(const point2d& p)
    {
      // Original pixel.
      return p.row() % 3 == 0 && p.col() % 3 == 0;
    }

    inline
    bool is_2_face_(short row, short col)
    {
      // Original pixel.
      return row % 3 == 0 && col % 3 == 0;
    }

    inline
    bool is_not_2_face(const point2d& p)
    {
      // Original pixel.
      return p.row() % 3 || p.col() % 3;
    }

    inline
    bool is_0_face(const point2d& p)
    {
      // Point in-between pixels.
      return p.row() % 3 && p.col() % 3;
    }

    inline
    bool is_not_0_face(const point2d& p)
    {
      // Either original pixel or edge.
      return p.row() % 3 == 0 || p.col() % 3 == 0;
    }

    inline
    bool is_1_face(const point2d& p)
    {
      // Edge between pixels.
      return (p.row() % 3 == 0) != (p.col() % 3 == 0);
    }

    inline
    unsigned face_dim(const point2d& p)
    {
      return (p.row() % 3 == 0) + (p.col() % 3 == 0);
    }


    // precise 0-face:
    //    top-left, top-right, bottom-left or bottom-right
    // w.r.t. its 2-face.

    //  0-tl  1-t  0-tr  
    //  1-l    2   1-r
    //  0-bl  1-b  0-br

    inline
    bool is_0_face_top_left(const point2d& p)
    {
      mln_precondition(is_0_face(p));
      return is_2_face_(p.row() + 1, p.col() + 1);
    }

    inline
    bool is_0_face_top_right(const point2d& p)
    {
      mln_precondition(is_0_face(p));
      return is_2_face_(p.row() + 1, p.col() - 1);
    }

    inline
    bool is_0_face_bottom_left(const point2d& p)
    {
      mln_precondition(is_0_face(p));
      return is_2_face_(p.row() - 1, p.col() + 1);
    }

    inline
    bool is_0_face_bottom_right(const point2d& p)
    {
      mln_precondition(is_0_face(p));
      return is_2_face_(p.row() - 1, p.col() - 1);
    }


    unsigned id_0_face(const point2d& p)
    {
      mln_precondition(is_0_face(p));
      //    | |
      // -- 0 1 --
      // -- 2 3 --
      //    | |
      return 2 * (p.row() - 1) % 3 + (p.col() - 1) % 3;
    }



    // precise 1-face (on right, bottom, left or on top)

    inline
    bool is_1_face_top(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      // a 2-face at the bottom of p
      return (p.row() + 1) % 3 == 0 && p.col() % 3 == 0;
    }

    inline
    bool is_1_face_bottom(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      // a 2-face on top of p
      return (p.row() - 1) % 3 == 0 && p.col() % 3 == 0;
    }

    inline
    bool is_1_face_left(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      // a 2-face at the right of p
      return p.row() % 3 == 0 && (p.col() + 1) % 3 == 0;
    }

    inline
    bool is_1_face_right(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      // a 2-face at the left of p
      return p.row() % 3 == 0 && (p.col() - 1) % 3 == 0;
    }

    inline
    bool is_1_face_vertical(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      return p.row() % 3 == 0;
    }

    inline
    bool is_1_face_horizontal(const point2d& p)
    {
      mln_precondition(is_1_face(p));
      return p.col() % 3 == 0;
    }


    unsigned id_1_face(const point2d& p)
    {
      mln_precondition(is_1_face(p));

      //     0
      //    ---
      // 2 |   | 3
      //    ---
      //     1

      if (p.col() % 3 == 0)
	// top or bottom
	return (p.row() - 1) % 3 == 0;
      else
	// left or right
	return 2 + ((p.col() - 1) % 3 == 0);
    }



    // Immersion.

    template <typename T>
    image2d<T>
    immerge(const image2d<T>& f)
    {
      image2d<T> K(3 * f.nrows() - 2, 3 * f.ncols() - 2);
      data__paste_values(f, (K | is_2_face).rw());
      return K;
    }


    // Neighborhood utilities.

    template <unsigned S>
    neighb2d
    make_asymmetric_neighb2d(const bool (&values)[S])
    {
      enum { h = mlc_sqrt_int(S) / 2 };
      mlc_bool((2 * h + 1) * (2 * h + 1) == S)::check();
      neighb2d nbh;
      window2d& win = nbh.hook_win_();
      convert::from_to(values, win);
      return nbh;
    }


    neighb2d
    rotate_90(const neighb2d& nbh)
    {
      neighb2d nbh_;
      window2d& win = nbh_.hook_win_();
      for (unsigned i = 0; i < nbh.size(); ++i)
	win.insert(- nbh.dp(i).col(), nbh.dp(i).row());
      return nbh_;
    }


    template <typename I, typename N>
    void
    display_nbh(const I& ima, const point2d& p, const N& nbh)
    {
      std::cout << ima(p) << ":  ";
      mln_niter(N) n(nbh, p);
      for_all(n)
	if (ima.domain().has(n))
	  std::cout << ima(n) << ' ';
      std::cout << std::endl;
    }


    template <typename I, typename F, typename N>
    void
    test_nbh(const std::string& msg,
	     const I& f,
	     const F& is_face,
	     const N& nbh)
    {
      image2d<int> K = immerge(f);
      debug::iota(K);

//       // default is 0 for all faces except...
//       data::fill(K, 0);
//       // ... 2-faces filled with -iota.
//       debug::iota((K | is_2_face).rw());
//       data::fill(K, - pw::value(K));

//       // overriden by:
//       debug::iota((K | is_face).rw());

      std::cout << msg << std::endl
		<< std::endl;
      debug::println(K);
      
      mln_VAR(d, (K | is_face).domain());
      mln_piter(d_t) p(d);
      for_all(p)
	display_nbh(K, p, nbh);
      std::cout << std::endl;
    }



    // Neighborhoods.

    neighb2d
    nbh_2_face()
    {
      bool vals[] = { 0, 0, 0, 1, 0, 0, 0,
		      0, 0, 0, 0, 0, 0, 0,
		      0, 0, 0, 0, 0, 0, 0,
		      1, 0, 0, 0, 0, 0, 1,
		      0, 0, 0, 0, 0, 0, 0,
		      0, 0, 0, 0, 0, 0, 0,
		      0, 0, 0, 1, 0, 0, 0 };
      return make::neighb2d(vals);
    }

    neighb2d
    nbh_1_face_right()
    {
      bool vals[] = { 0, 0, 0, 1, 0, 0, 0,
		      0, 0, 0, 0, 0, 0, 0,
		      0, 0, 1, 0, 0, 0, 0,
		      0, 0, 0, 0, 1, 0, 0,
		      0, 0, 1, 0, 0, 0, 0,
		      0, 0, 0, 0, 0, 0, 0,
		      0, 0, 0, 1, 0, 0, 0 };
      return make_asymmetric_neighb2d(vals);
    }

    neighb2d
    nbh_1_face_top()
    {
      return rotate_90(nbh_1_face_right());
    }

    neighb2d
    nbh_1_face_left()
    {
      return rotate_90(nbh_1_face_top());
    }

    neighb2d
    nbh_1_face_bottom()
    {
      return rotate_90(nbh_1_face_left());
    }


    inline
    neighb< win::multiple<window2d, unsigned (*)(const point2d&)> >
    nbh_1_face()
    {
      typedef unsigned (*F)(const point2d&);
      typedef win::multiple<window2d, F> W;
      W win(id_1_face);
      //     0
      //    ---
      // 2 |   | 3
      //    ---
      //     1
      win.set_window(0, nbh_1_face_top().win());
      win.set_window(1, nbh_1_face_bottom().win());
      win.set_window(2, nbh_1_face_left().win());
      win.set_window(3, nbh_1_face_right().win());
      neighb<W> nbh(win);
      return nbh;
    }



    // Neighborhoods through dimensions.

    inline
    neighb2d
    nbh_2_to_1()
    {
      //   .
      // . o .
      //   .
      return c4();
    }

    inline
    neighb< win::multiple<window2d, unsigned (*)(const point2d&)> >
    nbh_1_to_2()
    {
      typedef unsigned (*F)(const point2d&);
      typedef win::multiple<window2d, F> W;
      W win(id_1_face);
      //     0
      //    ---
      // 2 |   | 3
      //    ---
      //     1
      win.set_window( 0, window2d().insert(dpoint2d(+1,0)) );
      win.set_window( 1, window2d().insert(dpoint2d(-1,0)) );
      win.set_window( 2, window2d().insert(dpoint2d(0,+1)) );
      win.set_window( 3, window2d().insert(dpoint2d(0,-1)) );

      neighb<W> nbh(win);
      return nbh;
    }


    inline
    neighb< win::multiple<window2d, unsigned (*)(const point2d&)> >
    nbh_0_to_1()
    {
      typedef unsigned (*F)(const point2d&);
      typedef win::multiple<window2d, F> W;
      W win(id_0_face);
      //    | |
      // -- 0 1 --
      // -- 2 3 --
      //    | |
      win.set_window( 0, window2d().insert(dpoint2d(-1,0)).insert(dpoint2d(0,-1)) );
      win.set_window( 1, window2d().insert(dpoint2d(-1,0)).insert(dpoint2d(0,+1)) );
      win.set_window( 2, window2d().insert(dpoint2d(+1,0)).insert(dpoint2d(0,-1)) );
      win.set_window( 3, window2d().insert(dpoint2d(+1,0)).insert(dpoint2d(0,+1)) );

      neighb<W> nbh(win);
      return nbh;
    }
    


    // FIXME: condition commented


// a.out: ./+inc/mln/core/internal/neighb_base.hh:109: void mln::internal::neighb_base<W, E>::change_window(const W&) [with W = mln::win::multiple<mln::window<mln::dpoint<mln::grid::square, short int> >, unsigned int (*)(const mln::point2d&)>, E = mln::neighb<mln::win::multiple<mln::window<mln::dpoint<mln::grid::square, short int> >, unsigned int (*)(const mln::point2d&)> >]: Assertion `(bool)(new_win.is_neighbable_

//     template <typename W, typename E>
//     inline
//     void
//     neighb_base<W,E>::change_window(const W& new_win)
//     {
// //       mln_precondition(new_win.is_neighbable_());
//       win_ = new_win;
//     }





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



    // Immersion as range.


    template <typename R>
    inline
    void set_1_face_range(image2d<R>& K,
			  const point2d& p,
			  const mln_value(R)& v_p,
			  const dpoint2d& dp,
			  const mln_value(R)& v_n)
    {
      if (v_n == v_p)
	{
	  K(p + dp) = R(v_p, v_p);
	  K(p + 2 * dp) = R(v_p, v_p);
	}
      else
	if (v_n > v_p)
	  {
	    K(p + dp) = R(v_p, v_n - 1);
	    K(p + 2 * dp) = R(v_p + 1, v_n);
	  }
	else // v_n < v_p
	  {
	    K(p + dp) = R(v_n + 1, v_p);
	    K(p + 2 * dp) = R(v_n, v_p - 1);
	  }
    }


    template <typename T>
    image2d< range<T> >
    immerge_as_range(const image2d<T>& f)
    {
      box2d b = f.domain();

      typedef range<T> R;
      image2d<R> K(3 * f.nrows() - 2, 3 * f.ncols() - 2);
      
      mln_piter(box2d) p(b);
      for_all(p)
      {
	point2d p_(3 * p.row(), 3 * p.col());
	K(p_) = R(f(p), f(p));

	if (b.has(p + up))    set_1_face_range(K, p_, f(p), up,    f(p + up));
	if (b.has(p + down))  set_1_face_range(K, p_, f(p), down,  f(p + down));
	if (b.has(p + left))  set_1_face_range(K, p_, f(p), left,  f(p + left));
	if (b.has(p + right)) set_1_face_range(K, p_, f(p), right, f(p + right));
      }
      return K;
    }



    template <typename T>
    void
    add_0_faces_range(image2d< range<T> >& K)
    {
      mln_piter(box2d) p(K.domain());
      for_all(p)
      {
	if (is_not_0_face(p))
	  continue;
	if (is_0_face_top_left(p))
	  K(p) = uni(K(p + down), K(p + right));
	else
	  if (is_0_face_top_right(p))
	    K(p) = uni(K(p + down), K(p + left));
	  else
	    if (is_0_face_bottom_left(p))
	      K(p) = uni(K(p + up), K(p + right));
	    else
	      if (is_0_face_bottom_right(p))
		K(p) = uni(K(p + up), K(p + left));
	      else
		mln_invariant(false);
      }
    }


    // pretty-print

    void
    println(const std::string& msg,
	    const image2d<bool>& K,
	    std::ostream& ostr = std::cout)
    {
      ostr << msg << std::endl;
      for (int row = 0; row < K.nrows(); ++row)
	{
	  for (int col = 0; col < K.ncols(); ++col)
	    {
	      point2d p(row, col);
	      if (K(p) == false)
		{
		  ostr << "  ";
		  continue;
		}
	      if (is_2_face(p))
		ostr << "O ";
	      else
		if (is_1_face(p))
		  ostr << (is_1_face_vertical(p) ? "| " : "- ");
		else
		  ostr << ". ";
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }



    // Sets.


    template <typename I, typename T>
    mln_ch_value(I, bool)
      level_set(const Image<I>& input_, const T& v)
    {
      const I& input = exact(input_);
      mln_ch_value(I, bool) output;
      initialize(output, input);
      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = input(p) == v;
      return output;
    }

    template <typename I, typename T>
    mln_ch_value(I, bool)
      upper_set(const Image<I>& input, const T& v)
    {
      return duplicate((pw::value(input) >= pw::cst(v)) | exact(input).domain());
//       const I& input = exact(input_);
//       mln_ch_value(I, bool) output;
//       initialize(output, input);
//       mln_piter(I) p(input.domain());
//       for_all(p)
// 	output(p) = input(p) >= v;
//       return output;
    }

    template <typename I, typename T>
    mln_ch_value(I, bool)
      lower_set(const Image<I>& input_, const T& v)
    {
      const I& input = exact(input_);
      mln_ch_value(I, bool) output;
      initialize(output, input);
      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = input(p) <= v;
      return output;
    }

    template <typename I, typename T>
    mln_ch_value(I, bool)
      upper_set_strict(const Image<I>& input_, const T& v)
    {
      const I& input = exact(input_);
      mln_ch_value(I, bool) output;
      initialize(output, input);
      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = input(p) > v;
      return output;
    }

    template <typename I, typename T>
    mln_ch_value(I, bool)
      lower_set_strict(const Image<I>& input_, const T& v)
    {
      const I& input = exact(input_);
      mln_ch_value(I, bool) output;
      initialize(output, input);
      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = input(p) < v;
      return output;
    }



    // Sets.


//     template <typename I, typename T>
//     mln_ch_value(I, bool)
//       upper_set_v2(const Image<I>& input_, const T& v)
//     {
//       const I& input = exact(input_);
//       mln_ch_value(I, bool) output;
//       initialize(output, input);
//       data::fill(output, false);
//       mln_piter(I) p(input.domain());
//       mln_niter(neighb2d) n(c8(), p);
//       for_all(p)
//       {
// 	if (is_2_face(p))
// 	  {
// 	    bool ok = input(p) >= v;
// 	    output(p) = ok;
// 	    if (ok)
// 	      for_all(n)
// 		output(n) = ok;
// 	  }
//       }
//       return output;
//     }


    image2d<bool>
    elementary_dilation_from_2_to_1_and_0(const image2d<bool>& input)
    {
      image2d<bool> output;
      initialize(output, input);
      data::paste(morpho::dilation(extend(output | is_1_face,
					  output),
				   nbh_1_to_2().win()),
		  output);
      data::paste(morpho::dilation(extend(output | is_0_face,
					  output),
				   nbh_0_to_1().win()),
		  output);
      return output;
    }



    image2d<bool>
    dilation_2_to_1_0(const image2d<bool>& input)
    {
      image2d<bool> output = duplicate(input);
      data::paste(morpho::dilation(extend(output | is_1_face,
					  output),
				   nbh_1_to_2().win()),
		  output);
      data::paste(morpho::dilation(extend(output | is_0_face,
					  output),
				   nbh_0_to_1().win()),
		  output);
      return output;
    }



    template <typename I, typename T>
    mln_ch_value(I, bool)
      upper_set_v2(const Image<I>& input, const T& v)
    {
      image2d<bool> output;
      initialize(output, input);

      // Dummy code:
      //       data::fill(output, false);
      //       data::fill((output | is_2_face).rw(), pw::value(input) >= pw::cst(v));
      //       output = morpho::elementary::dilation(output, c8());
      // replace by the more interesting one:

      data::fill((output | is_2_face).rw(), pw::value(input) >= pw::cst(v));
      output = dilation_2_to_1_0(output);

      return output;
    }


    template <typename I, typename T>
    mln_ch_value(I, bool)
      lower_set_strict_v2(const Image<I>& input, const T& v)
    {
      image2d<bool> output;
      initialize(output, input);

      data::fill((output | is_2_face).rw(), pw::value(input) < pw::cst(v));
      output = dilation_2_to_1_0(output);

      return output;
    }


    template <typename I>
    mln_ch_value(I, bool)
      contour(const Image<I>& input)
    {
      return morpho::elementary::gradient_internal(input, c8());
    }


  } // end of namespace mln::kk2d

} // end of namespace mln


#endif // ! KK2D_HH
