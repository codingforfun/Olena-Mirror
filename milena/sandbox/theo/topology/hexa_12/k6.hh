#ifndef K6_HH
# define K6_HH


#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/fill.hh>

#include "range.hh"
#include "dh_queue.hh"


namespace mln
{

  namespace k6
  {

    //     0 1 2 3
    //     -------
    // 0 | a b c d
    // 1 | e f g h
    // 2 | i j k l


    //     0 1 2 3 4 5 6 7 8
    //     -----------------
    // 0 | \ - / x \ - / x \
    // 1 | / a \ - / c \ - /
    // 2 | \ - / b \ - / d \
    // 3 | / e \ - / g \ - /
    // 4 | \ - / f \ - / h \
    // 5 | / i \ - / k \ - /
    // 6 | \ - / j \ - / l \
    // 7 | / x \ - / x \ - /



    // Immersion.

    box2d to_K(const box2d& b)
    {
      box2d tmp(2 * b.nrows() + 2, 2 * b.ncols() + 1);
      return tmp;
    }

    inline
    point2d to_K(const point2d& p)
    {
      point2d tmp(2 * p.row() + 1 + (p.col() % 2), 2 * p.col() + 1);
      return tmp;
    }

    inline
    point2d from_K(const point2d& p)
    {
      point2d tmp((p.row() - 1) / 2, (p.col() - 1) / 2);
      return tmp;
    }

    box2d from_K(const box2d& b)
    {
      box2d tmp((b.nrows() - 2) / 2, (b.ncols() - 1) / 2);
      return tmp;
    }


    template <typename T, typename U>
    image2d<T> immerge(const image2d<T>& ima,
		       const U& default_value)
    {
      const box2d& b = ima.domain();
      image2d<T> K(to_K(b));
      data::fill(K, default_value);
      mln_piter(box2d) p(b);
      for_all(p)
	K(to_K(p)) = ima(p);
      return K;
    }



    // Faces predicates.

    typedef fun::C<bool (*)(const mln::point2d&)> predicate_t;
 


    //     0 1 2 3 4 5 6 7 8
    //     -----------------
    // 0 | \ - / x \ - / x \
    // 1 | / a \ - / c \ - /
    // 2 | \ - / b \ - / d \
    // 3 | / e \ - / g \ - /
    // 4 | \ - / f \ - / h \
    // 5 | / i \ - / k \ - /
    // 6 | \ - / j \ - / l \
    // 7 | / x \ - / x \ - /

    inline
    bool is_2_face(const point2d& p)
    {
      return p.col() % 2 == 1 && (p.col() / 2) % 2 != p.row() % 2;
    }

    inline
    bool is_2_face_(int row, int col)
    {
      return col % 2 == 1 && (col / 2) % 2 != row % 2;
    }




    inline
    bool is_1_face(const point2d& p)
    {
      return p.col() % 2 == 0 || (p.col() / 2) % 2 == p.row() % 2;
    }



    inline
    bool is_1_face_horiz(const point2d& p)
    {
      //  -
      //  o
      return is_2_face_(p.row() + 1, p.col());
    }

    inline
    bool is_1_face_diag(const point2d& p)
    {
      //  / o
      return is_2_face_(p.row(), p.col() + 1);
    }

    inline
    bool is_1_face_anti_diag(const point2d& p)
    {
      //   o \   .
      return is_2_face_(p.row(), p.col() - 1);
    }


    inline
    bool is_inside(const point2d& p, const box2d& b)
    {
      return
	p.row() > 0 && p.row() < b.pmax().row() &&
	p.col() > 0 && p.col() < b.pmax().col();
    }


    inline
    bool is_external_border(const point2d& p, const box2d& b)
    {
      //     0 1 2 3 4 5 6 7 8
      //     -----------------
      // 0 | \ _ / x \ _ / x \  <- tous
      // 1 | / a \ _ / c \ _ /  <- les 1-faces 
      // 2 | \ _ / b \ _ / d \  .
      // 3 | / e \ _ / g \ _ /
      // 4 | \ _ / f \ _ / h \  .
      // 5 | / i \ _ / k \ _ /
      // 6 | \ _ / j \ _ / l \  <- que les '-'
      // 7 | / x \ _ / x \ _ /  <- tous
      return
	p.col() == 0 || p.col() == b.pmax().col() ||
	p.row() == 0 || p.row() == b.pmax().row() ||
	(p.row() == 1 && p.col() % 4 != 1) ||
	(p.row() == b.pmax().row() - 1 && p.col() % 4 == 1);
    }




    // Pretty-printers.

    //     0 1 2 3 4 5 6 7 8
    //     -----------------
    // 0 | \ _ / x \ _ / x \
    // 1 | / a \ _ / c \ _ /
    // 2 | \ _ / b \ _ / d \
    // 3 | / e \ _ / g \ _ /
    // 4 | \ _ / f \ _ / h \
    // 5 | / i \ _ / k \ _ /
    // 6 | \ _ / j \ _ / l \
    // 7 | / x \ _ / x \ _ /


    void
    pretty_println(const std::string& msg,
		   const image2d<bool>& msk,
		   std::ostream& ostr = std::cout)
    {
      if (msg != "")
	ostr << msg << std::endl;
      const int nrows = msk.nrows(), ncols = msk.ncols();
      point2d p;
      short& row = p.row();
      short& col = p.col();
      for (row = 0; row < nrows; ++row)
	{
	  for (col = 0; col < ncols; ++col)
	    {
// 	      if (! is_inside(p, msk.domain()))
// 		{
// 		  ostr << "  ";
// 		  continue;
// 		}
	      if (is_2_face(p))
		ostr << (msk(p) ? 'x' : ' ');
	      else if (is_1_face_horiz(p))
		ostr << (msk(p) ? '_' : ' ');
	      else if (is_1_face_diag(p))
		ostr << (msk(p) ? '/' : ' ');
	      else
		ostr << (msk(p) ? '\\' : ' ');
	      ostr << ' ';
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }


    void
    pretty_println(const image2d<bool>& ima,
		   std::ostream& ostr = std::cout)
    {
      pretty_println("", ima, ostr);
    }



    template <typename T>
    std::string
    format(const T& v, unsigned len)
    {
      std::string str(len, ' ');
      switch (len)
      {
      case 1:
	str[0] = '0' + v % 10;
	break;
      case 2:
	if (v >= 10)
	  str[0] = '0' + v / 10;
	str[1] = '0' + v % 10;
	break;
      case 3:
	str[0] = '0' + v / 100;
	str[1] = '0' + (v / 10) % 10;
	str[2] = '0' + v % 10;
	break;
      }
      return str;
    }

    template <typename T>
    void
    pretty_println(const std::string& msg,
		   const image2d<T>& ima,
		   unsigned len,
		   std::ostream& ostr = std::cout)
    {
      std::string spc(len, ' ');

      if (msg != "")
	ostr << msg << std::endl;
      const int nrows = ima.nrows(), ncols = ima.ncols();
      point2d p;
      short& row = p.row();
      short& col = p.col();
      for (row = 0; row < nrows; ++row)
	{
	  for (col = 0; col < ncols; ++col)
	    {
	      if (is_2_face(p))
		ostr << (is_inside(p, ima.domain()) ?
			 format(ima(p), len) :
			 spc);
	      else if (is_1_face_horiz(p))
		{
		  for (unsigned i = 0; i < len; ++i)
		    ostr << '_';
		}
	      else if (is_1_face_diag(p))
		ostr << '/';
	      else
		ostr << '\\';

	      ostr << ' ';
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }


    template <typename T>
    void
    pretty_println(const image2d<T>& ima,
		   unsigned len,
		   std::ostream& ostr = std::cout)
    {
      pretty_println("", ima, len, ostr);
    }



    template <typename I>
    void
    set_1_face(I& Kr,
	       const point2d& p,
	       const dpoint2d& dp1, const dpoint2d& dp2)
    {
      point2d
	p1 = p + dp1,
	p2 = p + dp2;
      if (is_inside(p1, Kr.domain())
	  && is_inside(p2, Kr.domain()))
	{
	  Kr(p).lower = std::min(Kr(p1).lower, Kr(p2).lower);
	  Kr(p).upper = std::max(Kr(p1).upper, Kr(p2).upper);
	}
    }


    template <typename T>
    image2d< range<T> >
    to_range(const image2d<T>& K)
    {
      typedef dpoint2d D;
      typedef range<T> R;
      R nil(0, 0);

      box2d b = K.domain();
      image2d<R> Kr(b);
      mln_piter(box2d) p(b);

      // 2-faces.
      for_all(p)
	if (is_external_border(p, b))
	  Kr(p) = nil;
	else if (is_2_face(p))	
	  Kr(p) = R(K(p), K(p));

      // 1-faces.
      for_all(p)
      {
	if (is_1_face_horiz(p))
	  //  o
	  //  -
	  //  o
	  set_1_face(Kr, p, D(-1, 0), D(+1, 0));
	else if (is_1_face_diag(p))
	  //  o
	  //    / o
	  set_1_face(Kr, p, D(-1, -1), D(0, +1));
	else if (is_1_face_anti_diag(p))
	  //      o
	  //  o \    .
	  set_1_face(Kr, p, D(0, -1), D(-1, +1));
      }

      return Kr;
    }


    template <typename T>
    T
    local_grad(const image2d<T>& K,
	       const point2d& p,
	       const dpoint2d& dp1, const dpoint2d& dp2)
    {
      point2d
	p1 = p + dp1,
	p2 = p + dp2;
      if (is_inside(p1, K.domain()) && is_inside(p2, K.domain()))
	return K(p1) > K(p2) ? K(p1) - K(p2) : K(p2) - K(p1);
      else
	return 0;
    }


    template <typename T>
    image2d<T>
    grad(const image2d<T>& K)
    {
      typedef dpoint2d D;

      box2d b = K.domain();
      image2d<T> out(b);
      data::fill(out, 0);

      mln_piter(box2d) p(b);
      for_all(p)
	if (is_1_face_horiz(p))
	  out(p) = local_grad(K, p, D(-1,  0), D(+1,  0));
	else if (is_1_face_diag(p))
	  out(p) = local_grad(K, p, D(-1, -1), D( 0, +1));
	else if (is_1_face_anti_diag(p))
	  out(p) = local_grad(K, p, D( 0, -1), D(-1, +1));
      
      return out;
    }


    // Neighborhood.


    typedef neighb< win::multiple<window2d, bool (*)(const mln::point2d&)> > nbh_t;


    inline
    bool is_col_odd(const point2d& p)
    {
      return p.col() % 2 == 1;
    }


    nbh_t nbh()
    {
      static bool  odd[] = { 0, 1, 0,
			     1, 0, 1,
			     1, 1, 1 };

      static bool even[] = { 1, 1, 1,
			     1, 0, 1,
			     0, 1, 0 };

      static nbh_t it = make::double_neighb2d(is_col_odd, odd, even);
      return it;
    }




  } // mln::k6

} // mln


#endif // ! K6_HH
