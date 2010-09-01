
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>
#include <mln/extension/fill.hh>

#include "../utils/dh_queue.hh"
#include "../utils/h_median.hh"




namespace mln
{

  namespace k4
  {

    

    template <typename T>
    T
    frontiere_median(const image2d<T>& f)
    {
      utils::h_median<T> m;
      const int
	nrows = f.nrows(),
	nrows_1 = nrows - 1,
	ncols = f.ncols(),
	ncols_1 = ncols - 1;

      // ------
      //
      // ------
      for (int col = 0; col < ncols; ++col)
	{
	  m.take(f.at_(0, col));
	  m.take(f.at_(nrows_1, col));
	}

      // ------
      // |    |
      // ------
      for (int row = 1; row < nrows_1; ++row)
	{
	  m.take(f.at_(row, 0));
	  m.take(f.at_(row, ncols_1));
	}

      return m.value();
    }



    template <typename R>
    void
    set_frontiere(image2d<R>& K, const R& r)
    {
      const int
	nrows = K.nrows(),
	nrows_1 = nrows - 1,
	ncols = K.ncols(),
	ncols_1 = ncols - 1;

      for (int col = 0; col < ncols; ++col)
	{
	  K.at_(0, col) = r;
	  K.at_(nrows_1, col) = r;
	}
      for (int row = 1; row < nrows_1; ++row)
	{
	  K.at_(row, 0) = r;
	  K.at_(row, ncols_1) = r;
	}
    }



    //      0 1 2 
    //
    //  0   a b c
    //  1   d e f


    //      0 1 2 3 4 5 6
    //
    //  0   + - + - + - +
    //  1   | a | b | c |
    //  2   + - + - + - +
    //  3   | d | e | f |
    //  4   + - + - + - +


    inline
    bool
    is_2_face(const point2d& p)
    {
      return p.row() % 2 && p.col() % 2;
    }

    inline
    bool
    is_1_face(const point2d& p)
    {
      return p.row() % 2 != p.col() % 2;
    }

    inline
    bool
    is_0_face(const point2d& p)
    {
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }


    inline
    bool
    is_not_2_face(const point2d& p)
    {
      return p.row() % 2 == 0 || p.col() % 2 == 0;
    }


    inline
    bool
    is_1_face_horizontal(const point2d& p)
    {
      return p.row() % 2 == 0 && p.col() % 2;
    }

    inline
    bool
    is_1_face_vertical(const point2d& p)
    {
      return p.row() % 2 && p.col() % 2 == 0;
    }


    inline
    bool
    is_frontiere_face(const point2d& p, const box2d& b)
    {
      return
	p.row() == b.pmin().row() ||
	p.row() == b.pmax().row() ||
	p.col() == b.pmin().col() ||
	p.col() == b.pmax().col();
    }



    void
    pretty_println(const image2d<bool>& B,
		   std::ostream& ostr = std::cout)
    {
      const int nrows = B.nrows(), ncols = B.ncols();
      point2d p;
      short& row = p.row();
      short& col = p.col();
      for (row = 0; row < nrows; ++row)
	{
	  for (col = 0; col < ncols; ++col)
	    {
	      if (is_2_face(p))
		ostr << (B(p) ? 'x' : ' ');
	      else if (is_1_face_horizontal(p))
		ostr << (B(p) ? '_' : ' ');
	      else if (is_1_face_vertical(p))
		ostr << (B(p) ? '|' : ' ');
	      else if (is_0_face(p))
		ostr << (B(p) ? '.' : ' ');
	      ostr << ' ';
	    }
	  ostr << std::endl;
	}
    }


    void
    pretty_println(const std::string& msg, const image2d<bool>& B,
		   std::ostream& ostr = std::cout)
    {
      ostr << msg << std::endl;
      pretty_println(B, ostr);
    }





    inline
    point2d
    immerse(const point2d& p)
    {
      point2d p_(2 * p.row() + 1, 2 * p.col() + 1);
      mln_postcondition(is_2_face(p_));
      return p_;
    }

    inline
    point2d
    unimmerse(const point2d& p)
    {
      mln_precondition(is_2_face(p));
      point2d p_(p.row() / 2, p.col() / 2);
      return p_;
    }


    //      0 1 2 
    //
    //  0   a b c
    //  1   d e f


    //      0 1 2 3 4 5 6
    //
    //  0   + - + - + - +
    //  1   | a | b | c |
    //  2   + - + - + - +
    //  3   | d | e | f |
    //  4   + - + - + - +


    template <typename T>
    image2d< utils::range<T> >
    immerse(const image2d<T>& f)
    {
      typedef utils::range<T> R;
      image2d<R> K(2 * f.nrows() + 1, 2 * f.ncols() + 1);
      mln_piter(box2d) p(f.domain());
      for_all(p)
	K(immerse(p)).set(f(p));
      return K;
    }


    template <typename T>
    void
    interpolate(image2d< utils::range<T> >& K)
    {
      typedef utils::range<T> R;
      const int
	nrows = K.nrows(),
	nrows_1 = nrows - 1,
	ncols = K.ncols(),
	ncols_1 = ncols - 1;
      int row, col;

      // Interpolate 1-faces.

      row = 1;
      for (;;) // for (row = 1; row < nrows_2; ++row)
	{
	  // |
	  for (col = 2; col < ncols_1; col += 2)
	    K.at_(row, col) = K.at_(row, col - 1) || K.at_(row, col + 1);
	  ++row;
	  if (row == nrows_1)
	    break;
	  // -
	  for (col = 1; col < ncols_1; col += 2)
	    K.at_(row, col) = K.at_(row - 1, col) || K.at_(row + 1, col);
	  ++row;
	}

      // Interpolate 0-faces.

      for (row = 2; row < nrows_1; row += 2)
	for (col = 2; col < ncols_1; col += 2)
	  K.at_(row, col) =
	    K.at_(row - 1, col) ||
	    K.at_(row, col - 1) || K.at_(row, col + 1) ||
	    K.at_(row + 1, col);
    }




    // -----------------------------------------   tree



    inline
    point2d z_find_root(image2d<point2d>& zpar, point2d x)
    {
      if (zpar(x) == x)
	return x;
      return zpar(x) = z_find_root(zpar, zpar(x));
    }


    void print_tree_stats(image2d<point2d>& parent, const std::vector<point2d>& S)
    {
      point2d p;
      image2d<bool> is_leaf(parent.domain());
      data::fill(is_leaf, true);
      for (int i = 0; i < S.size(); ++i)
	{
	  p = S[i];
	  is_leaf(parent(p)) = false;
	}
      unsigned nleaves_0 = 0, nleaves_1 = 0, nleaves_2 = 0;
      for (int i = 0; i < S.size(); ++i)
	{
	  p = S[i];
	  if (is_leaf(p))
	    {
	      if (is_0_face(p))
		++nleaves_0;
	      else if (is_1_face(p))
		++nleaves_1;
	      else
		++nleaves_2;
	    }
	}
      std::cout << "nleaves = " << (nleaves_0 + nleaves_1 + nleaves_2)
		<< " (#0=" << nleaves_0 << " #1=" << nleaves_1 << " #2=" << nleaves_2 << ")"
		<< std::endl;
    }
    


    // -----------------------------------------   end of tree routines




    template <typename R, typename N>
    void
    compute_autodual_tree(image2d<R>& K, const mln_value(R)& l_start, const N& nbh)
    {
      typedef point2d P;
      typedef mln_value(R) T;
      

      // Initialisation.
      // ---------------------------------------------

      const box2d b = K.domain();
      const unsigned n_faces = b.nsites();


      // K_

      image2d<T> K_(b);

      // S

      std::vector<P> S;
      S.reserve(b.nsites());

      image2d<int> ordering(b);
      data::fill(ordering, 0);
      unsigned index = 0;

      // q

      utils::dh_queue<P,T> q;
      q.init(l_start, false); // so going down.
      image2d<bool> in_queue(b);
      data::fill(in_queue, false);

      {

	mln_piter(box2d) p(b);
	for_all(p)
	  if (is_frontiere_face(p, b))
	    {
	      q.push(p, K(p));
	      in_queue(p) = true;
	    }
      }



      //  1st pass: ordering.
      // ---------------------------------------------


      // deja_vu

      image2d<bool> deja_vu(b);
      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      P p;
      mln_niter(N) n(nbh, p);

      while (! q.is_empty())
	{
	  p = q.pop();
	  S.push_back(p);

	  K_(p) = q.level();
	  ordering(p) = ++index;
	    
	  mln_invariant(in_queue(p) && ! deja_vu(p));
	  for_all(n)
	    if (! deja_vu(n) && ! in_queue(n))
	      {
		q.push(n, K(n));
		in_queue(n) = true;
	      }
	  deja_vu(p)  = true;
	  in_queue(p) = false;
	  if (q.is_level_completed())
	    {
	      std::cout << std::endl
			<< "level = " << q.level() << std::endl;
	      pretty_println(deja_vu);
	      // pretty_println("in-queue:", in_queue);
	    }
	}


      // debug::println("K_", K_ | is_not_2_face);
      // debug::println("S", ordering);




      //  2nd pass: rough tree.
      // ---------------------------------------------



      image2d<P> parent(b), zpar(b);

      // image2d<bool> contour(b);
      // data::fill(contour, false);
      // image2d<data_t> dta(b);

      data::fill(deja_vu, false);
      unsigned n_trees = 0;

      T l = K_(S[n_faces - 1]);
      for (int i_face = n_faces - 1; i_face >= 0; --i_face)
	{
	  p = S[i_face];

	  if (K_(p) != l)
	    {
	      std::cout << "n_trees = " << n_trees;
	      pretty_println(" ", deja_vu);

	      // 	      std::cout << "contour at " << l;
	      // 	      pretty_println(" ", contour);

	      l = K_(p);
	    }

	  parent(p) = p;
	  zpar(p) = p;
	  ++n_trees;

	  for_all(n) if (deja_vu(n))
	    {
	      P r = z_find_root(zpar, n);
	      if (r != p)
		{
		  --n_trees;
		  parent(r) = p;
		  zpar(r) = p;
		  // dta(p) += dta(r);
		}
	    }
	  
	  deja_vu(p) = true;
	  
	}

      std::cout << "n_trees = " << n_trees;
      pretty_println(" ", deja_vu);



      //  3rd pass: tree cleaning.
      // ---------------------------------------------

      print_tree_stats(parent, S);

      for (int i = 0; i < S.size(); ++i)
	{
	  p = S[i];
	  P q = parent(p);
	  if (K_(parent(q)) == K_(q))
	    parent(p) = parent(q);
	}    

      print_tree_stats(parent, S);

    }


  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::cerr;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  using value::int_u8;
  typedef utils::range<int_u8> R;

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);
  debug::println("f", f);

  int_u8 m = frontiere_median(f);
  std::cout << "frontiere median = " << m << std::endl
	    << std::endl;

  image2d<R> K = immerse(f);
  debug::println("K", K);
  interpolate(K);
  set_frontiere(K, R(m,m));
  debug::println("K", K);

  compute_autodual_tree(K, m, c4());

}
