
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
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
		ostr << (B(p) ? '-' : ' ');
	      else if (is_1_face_vertical(p))
		ostr << (B(p) ? '|' : ' ');
	      else if (is_0_face(p))
		ostr << (B(p) ? '+' : ' ');
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
    image2d< utils::range<T> >
    immerse_alpha(const image2d<T>& f, int alpha)
    {
      typedef utils::range<T> R;
      image2d<R> K(2 * f.nrows() + 1, 2 * f.ncols() + 1);
      mln_piter(box2d) p(f.domain());
      for_all(p)
	{
	  int
	    l = int(f(p)) - alpha,
	    u = int(f(p)) + alpha;
	  if (l < int(mln_min(T))) l = mln_min(T);
	  if (u > int(mln_max(T))) u = mln_max(T);
	  K(immerse(p)) = R(l, u);
	}
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


    template <typename T>
    image2d<T>
    gradient(const image2d< utils::range<T> >& K)
    {
      T zero = 0;
      image2d<T> d(K.domain());
      mln_piter(box2d) p(K.domain());
      for_all(p)
	if (is_1_face(p))
	  d(p) = K(p).dist();
	else
	  d(p) = 0;
      return d;
    }


    template <typename T>
    image2d<T>
    only_2_faces(const image2d<T>& F)
    {
      image2d<T> out(F.nrows() / 2, F.ncols() / 2);
      mln_piter(box2d) p(out.domain());
      for_all(p)
	out(p) = F(immerse(p));
      return out;
    }



    // -----------------------------------------   tree



    inline
    point2d z_find_root(image2d<point2d>& zpar, point2d x)
    {
      if (zpar(x) == x)
	return x;
      return zpar(x) = z_find_root(zpar, zpar(x));
    }


    template <typename T>
    void print_tree_stats(const image2d<point2d>& parent,
			  const std::vector<point2d>& S,
			  const image2d<T>& K_)
    {

      // About leaves.
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
		  << "\t(#0=" << nleaves_0 << "\t#1=" << nleaves_1 << "\t#2=" << nleaves_2 << ")"
		  << std::endl;
      }

      // About nodes.
      {
	unsigned nnodes_0 = 0, nnodes_1 = 0, nnodes_2 = 0;
	mln_piter(box2d) p(parent.domain());
	for_all(p)
	{
	  if (parent(p) == p || K_(parent(p)) != K_(p))
	    {
	      if (is_0_face(p))
		++nnodes_0;
	      else if (is_1_face(p))
		++nnodes_1;
	      else
		++nnodes_2;
	    }
	}
	std::cout << "nnodes = " << (nnodes_0 + nnodes_1 + nnodes_2)
		  << "\t(#0=" << nnodes_0 << "\t#1=" << nnodes_1 << "\t#2=" << nnodes_2 << ")"
		  << std::endl;
      }

    }
    


    // -----------------------------------------   attributes



    struct data_t
    {
      data_t()
      {
	L_var = R_var = 0;
	L_len = R_area = 0;
      }
      unsigned L_var, R_var;
      unsigned L_len, R_area;

      unsigned L_var_mean() const
      {
	if (L_len == 0)
	  return 1;
	return L_var / L_len;
      }

      void operator+=(const data_t& d)
      {
	L_var += d.L_var;
	R_var += d.R_var;
	L_len += d.L_len;
	R_area += d.R_area;
      }
    };



    template <typename R>
    image2d<value::int_u8>
    depict_L_var_mean(const image2d<point2d>& parent,
		      const image2d<R>& K_,
		      const image2d<data_t>& dta)
    {
      image2d<value::int_u8> out(parent.domain());
      image2d<bool> safety(parent.domain());
      data::fill(safety, false);
      mln_piter_(box2d) p(parent.domain());
      for_all(p)
	if (parent(p) == p || K_(parent(p)) != K_(p)) // node
	  {
	    out(p) = dta(p).L_var_mean();
	    safety(p) = true;
	  }
      for_all(p)
	if (K_(parent(p)) == K_(p)) // not a node
	  {
	    point2d q = parent(p); // parent(p) is a node
	    if (! safety(q))
	      std::abort();
	    mln_invariant(parent(q) == q || K_(parent(q)) != K_(q));
	    out(p) = out(parent(p));
	  }
      return out;
    }


    template <typename R>
    image2d<value::int_u8>
    area_filter(const image2d<point2d>& parent,
		const std::vector<point2d>& S,
		const image2d<R>& K_,
		const image2d<data_t>& dta,
		unsigned lambda)
    {
      image2d<value::int_u8> out(parent.domain());
      const int n_faces = S.size();
      for (int i_face = 0; i_face < n_faces; ++i_face)
	{
	  point2d p = S[i_face];
	  if (parent(p) == p || K_(parent(p)) != K_(p))
	    {
	      // node
	      if (dta(p).R_area >= lambda)
		out(p) = K_(p);
	      else
		out(p) = out(parent(p));
	    }
	  else
	    out(p) = out(parent(p));
	}      

//       std::cout << "lambda = " << lambda << std::endl;
//       {
// 	image2d<unsigned> area(parent.domain());
// 	for (int i_face = 0; i_face < n_faces; ++i_face)
// 	  {
// 	    point2d p = S[i_face];
// 	    if (parent(p) == p || K_(parent(p)) != K_(p))
// 	      // node
// 	      area(p) = dta(p).R_area;
// 	    else
// 	      {
// 		point2d q = parent(p);
// 		if (! (parent(q) == q || K_(parent(q)) != K_(q)))
// 		  std::abort();
// 		area(p) = area(parent(p));
// 	      }
// 	  }
// 	debug::println("area", area);
//       }
//       debug::println("out", out);
//       debug::println("K_", K_);

      return out;
    }	 


    // -----------------------------------------   tree computation



    template <typename R, typename N>
    void
    compute_autodual_tree(image2d<R>& K, const mln_value(R)& l_start, const N& nbh,
			  unsigned lambda,
			  bool log = false)
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
	  if (log && q.is_level_completed())
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

      image2d<bool> contour(b);
      data::fill(contour, false);
      image2d<data_t> dta(b);

      data::fill(deja_vu, false);
      unsigned n_trees = 0;

      T l = K_(S[n_faces - 1]);
      for (int i_face = n_faces - 1; i_face >= 0; --i_face)
	{
	  p = S[i_face];

	  if (K_(p) != l)
	    {
	      if (log)
		{
		  std::cout << "n_trees = " << n_trees;
		  pretty_println(" ", deja_vu);
		  std::cout << "contour at " << l;
		  pretty_println(" ", contour);
		}
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
		  dta(p) += dta(r);
		}
	    }
	  
	  deja_vu(p) = true;

	  if (is_1_face(p))
	    {
	      if (contour(p))
		{
		  // exterior contour is now interior
		  contour(p) = false;
		  dta(p).L_var -= K(p).dist();
		  --dta(p).L_len;
		  dta(p).R_var += K(p).dist();
		} 
	    }
	  else if (is_2_face(p))
	    {
 	      ++dta(p).R_area;
	      for_all(n)
		if (! contour(n) && ! deja_vu(n))
		  {
		    contour(n) = true;
		    dta(p).L_var += K(n).dist();
		    ++dta(p).L_len;
		  }
	    }
	  
	}



      if (log)
	{
	  std::cout << "n_trees = " << n_trees;
	  pretty_println(" ", deja_vu);
	}



      //  3rd pass: canonized tree.
      // ---------------------------------------------

      if (log)
	{
	  std::cout << "before canonization:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}

      for (int i = 0; i < S.size(); ++i)
	{
	  P p = S[i],
	    q = parent(p),
	    r = parent(q);
	  if (K_(r) == K_(q))
	    parent(p) = r;
	}    

//       if (log)
	{
	  std::cout << "after canonization:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}



      //  Log.
      // ---------------------------------------------


//       io::pgm::save(depict_L_var_mean(parent, K_, dta),
// 		    "temp_L_var_mean.pgm");

      io::pgm::save(K_, "temp_K_.pgm");
      io::pgm::save(only_2_faces(K_), "temp_K.pgm");

//       io::pgm::save(data::stretch(value::int_u8(), ordering), "temp_S.pgm");
//       io::pgm::save(gradient(K), "temp_grad.pgm");

      image2d<value::int_u8> G = area_filter(parent, S, K_, dta, lambda);
      io::pgm::save(G, "temp_filtered_.pgm");
      io::pgm::save(only_2_faces(G), "temp_filtered.pgm");


    }




  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda log alpha" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 5)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  using value::int_u8;
  typedef utils::range<int_u8> R;

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);
  // debug::println("f", f);

  unsigned lambda = std::atoi(argv[2]);
  bool log = std::atoi(argv[3]);
  unsigned alpha = std::atoi(argv[4]);

  int_u8 m = frontiere_median(f);
  if (log)
    std::cout << "frontiere median = " << m << std::endl
	      << std::endl;

  image2d<R> K;
  if (alpha)
    K = immerse_alpha(f, alpha);
  else
    K = immerse_alpha(f, 0);
  // debug::println("K", K);

  interpolate(K);
  set_frontiere(K, R(m,m));
  // debug::println("K", K);

  compute_autodual_tree(K, m, c4(), lambda, log);

}
