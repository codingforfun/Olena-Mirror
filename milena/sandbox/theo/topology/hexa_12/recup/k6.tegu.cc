#ifndef K6_HH
# define K6_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/fill.hh>

#include "range.hh"


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


  namespace k6
  {

    // Faces predicates.

    //       0 1 2 3 4 5 6 7 8
    //    +-------------------
    //  0 |  + - +   + - +
    //  1 |  |   |   |   |
    //  2 |  + a + - + c + - +
    //  3 |  |   |   |   |   |
    //  4 |  + - + b + - + d +
    //  5 |  |   |   |   |   |
    //  6 |  + e + - + g + - + 
    //  7 |  |   |   |   |   |
    //  8 |  + - + f + - + h +
    //  9 |  |   |   |   |   |
    // 10 |  + i + - + k + - + 
    // 11 |  |   |   |   |   |
    // 12 |  + - + j + - + l +
    // 13 |      |   |   |   |
    // 14 |      + - +   + - +


    //       0 1 2 3 4 5 6 7 8
    //    +-------------------
    //  0 |  0 1 0   0 1 0
    //  1 |  1   1   1   1
    //  2 |  0 2 0 1 0 2 0 1 0
    //  3 |  1   1   1   1   1
    //  4 |  0 1 0 2 0 1 0 2 0
    //  5 |  1   1   1   1   1
    //  6 |  0 2 0 1 0 2 0 1 0 
    //  7 |  1   1   1   1   1
    //  8 |  0 1 0 2 0 1 0 2 0
    //  9 |  1   1   1   1   1
    // 10 |  0 2 0 1 0 2 0 1 0 
    // 11 |  1   1   1   1   1
    // 12 |  0 1 0 2 0 1 0 2 0
    // 13 |      1   1   1   1
    // 14 |      0 1 0   0 1 0


    typedef fun::C<bool (*)(const mln::point2d&)> predicate_t;


    inline
    bool is_2_face(const point2d& p)
    {
      return p.row() % 2 == 0 && p.col() % 2 == 1 && (p.row() / 2 + p.col() / 2) % 2;
    }


    inline
    bool is_1_face(const point2d& p)
    {
      if (p.row() % 2 == 1) // odd row => even col
	return p.col() % 2 == 0;

      // even row => odd col and...
      if (p.col() % 2 == 0)
	return false;

      // ...
      return (p.row() / 2 + p.col() / 2) % 2 == 0;
    }


    inline
    bool is_0_face(const point2d& p)
    {
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }


    inline
    bool is_a_face(const point2d& p)
    {
      return p.row() % 2 == 0 || p.col() % 2 == 0;
    }

    inline
    bool is_no_face(const point2d& p)
    {
      return p.row() % 2 == 1 && p.col() % 2 == 1;
    }


    inline
    unsigned face_dim(const point2d& p)
    {
      mln_precondition(is_a_face(p));
      if (is_0_face(p))
	return 0;
      if (is_1_face(p))
	return 1;
      if (is_2_face(p))
	return 2;
    }



    // kind of 1-face

    bool is_1_face_horiz(const point2d& p)     //  '-'
    {
      mln_precondition(is_1_face(p));
      return p.row() % 2 == 0;
    }

    bool is_1_face_diag(const point2d& p)      //  '/'
    {
      mln_precondition(is_1_face(p));
      return is_2_face(p + dpoint2d(1,1));
    }

    bool is_1_face_anti_diag(const point2d& p) //  '\'
    {
      mln_precondition(is_1_face(p));
      return is_2_face(p + dpoint2d(-1,1));
    }


    //       0 1 2 3 4 5 6 7 8
    //    +-------------------
    //  0 |  + - +   + - +
    //  1 |  |   |   |   |
    //  2 |  + a + - + c + - +
    //  3 |  |   |   |   |   |
    //  4 |  + - + b + - + d +
    //  5 |  |   |   |   |   |
    //  6 |  + e + - + g + - + 
    //  7 |  |   |   |   |   |
    //  8 |  + - + f + - + h +
    //  9 |  |   |   |   |   |
    // 10 |  + i + - + k + - + 
    // 11 |  |   |   |   |   |
    // 12 |  + - + j + - + l +
    // 13 |      |   |   |   |
    // 14 |      + - +   + - +




    // pretty printers


    std::string format_5(unsigned v)
    {
      //                 01234
      std::string tmp = "     ";
      if (v < 10)
	tmp[2] = '0' + v;
      else if (v < 100)
	{
	  tmp[2] = '0' + (v / 10);
	  tmp[3] = '0' + (v % 10);
	}
      else if (v < 1000)
	{
	  tmp[1] = '0' + (v / 100);
	  tmp[2] = '0' + ((v / 10) % 10);
	  tmp[3] = '0' + (v % 10);
	}
      return tmp;
    }

    std::string format_5(char v)
    {
      //                 43210
      std::string tmp = "     ";
      tmp[2] = v;
      return tmp;
    }


    void
    pretty_println(const std::string& msg,
		   const image2d<bool>& msk,
		   std::ostream& ostr = std::cout)
    {
      if (msg != "")
	ostr << msg << std::endl;
      const int nrows = msk.nrows(), ncols = msk.ncols();
      for (int row = 0; row < nrows; ++row)
	{
	  switch (row % 4)
	    {
	    case 0: ostr << "  "; break;
	    case 1:
	    case 3: ostr << ' '; break;
	    }
	  for (int col = 0; col < ncols; ++col)
	    {
	      point2d p(row, col);
	      if (is_no_face(p))
		ostr << "   ";
	      else if (is_2_face(p))
		{
		  bool ok = msk(p);
		  if (msk.domain().has(p + up))
		    ok = ok || msk(p + up);
		  if (msk.domain().has(p + down))
		    ok = ok || msk(p + down);
		  ostr << "  " << (ok ? "O" : " ") << "  ";
		}
	      else if (is_0_face(p))
		ostr << (msk(p) ? "+" : ".");
	      else if (is_1_face(p))
		{
		  if (msk(p))
		    {
		      if (is_1_face_horiz(p))
			ostr << "-";
		      else if (is_1_face_diag(p))
			ostr << "/";
		      else
			ostr << "\\";
		    }
		  else
		    ostr << ' ';
		}
	      ostr << ' ';
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }



    template <typename T>
    void
    pretty_println(const std::string& msg,
		   const image2d<T>&    ima,
		   const image2d<bool>& msk,
		   std::ostream& ostr = std::cout)
    {
      if (msg != "")
	ostr << msg << std::endl;
      const int nrows = ima.nrows(), ncols = ima.ncols();
      for (int row = 0; row < nrows; ++row)
	{
	  switch (row % 4)
	    {
	    case 0: ostr << "  "; break;
	    case 1:
	    case 3: ostr << ' '; break;
	    }
	  for (int col = 0; col < ncols; ++col)
	    {
	      point2d p(row, col);
	      if (is_no_face(p))
		ostr << "   ";
	      else if (is_2_face(p))
		{
		  if (msk(p))
		    ostr << format_5(ima(p));
		  else
		    ostr << "     ";
		}
	      else if (is_0_face(p))
		{
		  ostr << (msk(p) ? "+" : ".");
		}
	      else if (is_1_face(p))
		{
		  if (msk(p))
		    {
		      if (is_1_face_horiz(p))
			ostr << "-";
		      else if (is_1_face_diag(p))
			ostr << "/";
		      else
			ostr << "\\";
		    }
		  else
		    ostr << ' ';
		}
	      ostr << ' ';
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }


    template <typename T>
    void
    pretty_println(const std::string& msg,
		 const image2d<T>& ima,
		 std::ostream& ostr = std::cout)
    {
      ostr << msg << std::endl;
      const int nrows = ima.nrows(), ncols = ima.ncols();
      for (int row = 0; row < nrows; ++row)
	{
	  switch (row % 4)
	    {
	    case 0: ostr << "  "; break;
	    case 1:
	    case 3: ostr << ' '; break;
	    }
	  for (int col = 0; col < ncols; ++col)
	    {
	      point2d p(row, col);
	      if (is_no_face(p))
		ostr << "    ";
	      else if (is_2_face(p))
		ostr << format_5(ima(p)) << ' ';
	      else if (is_0_face(p))
		ostr << "+ ";
	      else if (is_1_face(p))
		{
		  if (is_1_face_horiz(p))
		    ostr << "- ";
		  else if (is_1_face_diag(p))
		    ostr << "/ ";
		  else
		    ostr << "\\ ";
		}
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }




//      000 1 222 3 444 5 666 7 888
//    +----------------------------
//  0 |   + - +       + - +
//  1 |  /     \     /     \
//  2 | +   a   + - +   c   + - +
//  3 |  \     /     \     /     \
//  4 |   + - +   b   + - +   d   +
//  5 |  /     \     /     \     /
//  6 | +   e   + - +   g   + - +
//  7 |  \     /     \     /     \
//  8 |   + - +   f   + - +   h   +
//  9 |  /     \     /     \     /
// 10 | +   i   + - +   k   + - +
// 11 |  \     /     \     /     \
// 12 |   + - +   j   + - +   l   +
// 13 |        \     /     \     /
// 14 |         + - +       + - +



    // neighborhoods

    neighb2d
    nbh_2_face()
    {
      bool vals[] = { 1, 0, 0, 0, 1,
		      0, 0, 0, 0, 0,
		      0, 0, 0, 0, 0,
		      0, 0, 0, 0, 0,
		      1, 0, 0, 0, 1 };
      return make::neighb2d(vals);
    }

    


    // Immersion.

    template <typename T, typename V>
    image2d<T>
    add_border(const image2d<T>& f, V border_value)
    {
      const int nrows = f.nrows(), ncols = f.ncols();
      image2d<T> f_(nrows + 2, ncols + 2);
      data::fill(f_, border_value);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  f_.at_(row + 1, col + 1) = f.at_(row, col);
      return f_;
    }

    template <typename T>
    image2d<T>
    immerge(const image2d<T>& f, T no_value = T(0))
    {
      const int nrows = f.nrows(), ncols = f.ncols();
      image2d<T> K(4 * f.nrows() + 3, 2 * f.ncols() + 1);
      data::fill(K, no_value);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  {
	    int
	      row_ = 4 * row + (col % 2 ? 4 : 2),
	      col_ = 2 * col + 1;
	    const T& v = f.at_(row, col);
	    K.at_(row_,     col_) = v;
	    K.at_(row_ - 1, col_) = v;
	    K.at_(row_ + 1, col_) = v;
	  }
      return K;
    }


    template <typename T>
    range<T>
    range_1_face(const image2d< range<T> >& f,
		 const point2d& p, const dpoint2d& dp1, const dpoint2d& dp2)
    {
      typedef range<T> R;
      const box2d& b = f.domain();
      point2d p1 = p + dp1, p2 = p + dp2;
      if (b.has(p1) && b.has(p2))
	return R(std::min(f(p1).lower,
			  f(p2).lower),
		 std::max(f(p1).upper,
			  f(p2).upper));
      if (b.has(p1))
	return f(p1);
      if (b.has(p2))
	return f(p2);
    }

    template <typename T>
    void
    extend_range(image2d< range<T> >& f)
    {
      typedef range<T> R;
      box2d b = f.domain();
      mln_piter(box2d) p(b);
      for_all(p)
      {
	if (! is_1_face(p))
	  continue;
	if (is_1_face_horiz(p))
	  f(p) = range_1_face(f, p, dpoint2d(-2, 0), dpoint2d(+2, 0));
	else if (is_1_face_diag(p))
	  f(p) = range_1_face(f, p, dpoint2d(-1,-1), dpoint2d(+1,+1));
	else
	  f(p) = range_1_face(f, p, dpoint2d(-1,+1), dpoint2d(+1,-1));
      }
      for_all(p)
      {
	if (! is_0_face(p))
	  continue;
	T lower = mln_max(T),
	  upper = mln_min(T);
	point2d n;
	n = p + dpoint2d(-1, 0);
	if (b.has(n))
	  {
	    if (f(n).lower < lower)
	      lower = f(n).lower;
	    if (f(n).upper > upper)
	      upper = f(n).upper;
	  }
	n = p + dpoint2d(+1, 0);
	if (b.has(n))
	  {
	    if (f(n).lower < lower)
	      lower = f(n).lower;
	    if (f(n).upper > upper)
	      upper = f(n).upper;
	  }
	n = p + dpoint2d( 0,-1);
	if (b.has(n))
	  {
	    if (f(n).lower < lower)
	      lower = f(n).lower;
	    if (f(n).upper > upper)
	      upper = f(n).upper;
	  }
	n = p + dpoint2d( 0,+1);
	if (b.has(n))
	  {
	    if (f(n).lower < lower)
	      lower = f(n).lower;
	    if (f(n).upper > upper)
	      upper = f(n).upper;
	  }
	f(p) = R(lower, upper);
      }
    }

    template <typename T>
    image2d< range<T> >
    to_range(const image2d<T>& f)
    {
      typedef range<T> R;
      box2d b = f.domain();
      image2d<R> f_(b);
      mln_piter(box2d) p(b);
      for_all(p)
	f_(p) = R(f(p), f(p));
      extend_range(f_);
      return f_;
    }



    //       0 1 2 3 4 5 6 7 8
    //    +-------------------
    //  0 |  + - +   + - +
    //  1 |  |   |   |   |
    //  2 |  + a + - + c + - +
    //  3 |  |   |   |   |   |
    //  4 |  + - + b + - + d +
    //  5 |  |   |   |   |   |
    //  6 |  + e + - + g + - + 
    //  7 |  |   |   |   |   |
    //  8 |  + - + f + - + h +
    //  9 |  |   |   |   |   |
    // 10 |  + i + - + k + - + 
    // 11 |  |   |   |   |   |
    // 12 |  + - + j + - + l +
    // 13 |      |   |   |   |
    // 14 |      + - +   + - +



    // Level lines.

    template <typename T, typename V>
    image2d<bool>
    level_lines(const image2d< range<T> >& f, V l) // line is at l + 0.5!
    {
      box2d b = f.domain();
      image2d<bool> ll(b);
      mln_piter(box2d) p(b);
      for_all(p)
      {
	if (is_2_face(p) || is_no_face(p))
	  {
	    ll(p) = false;
	    continue;
	  }
	mln_invariant(is_1_face(p) || is_0_face(p));
	ll(p) = (f(p) == l && f(p) == l + 1);
      }
      return ll;
    }

  } // mln::k6

} // mln



//      000 1 222 3 444 5 666 7 888
//    +----------------------------
//  0 |   + - +       + - +
//  1 |  /     \     /     \
//  2 | +   a   + - +   c   + - +
//  3 |  \     /     \     /     \
//  4 |   + - +   b   + - +   d   +
//  5 |  /     \     /     \     /
//  6 | +   e   + - +   g   + - +
//  7 |  \     /     \     /     \
//  8 |   + - +   f   + - +   h   +
//  9 |  /     \     /     \     /
// 10 | +   i   + - +   k   + - +
// 11 |  \     /     \     /     \
// 12 |   + - +   j   + - +   l   +
// 13 |        \     /     \     /
// 14 |         + - +       + - +



#endif // ! K6_HH
