
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
#include <mln/extension/fill.hh>

#include <mln/util/timer.hh>

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
    unsigned
    face_id(const point2d& p)
    {
      if (is_1_face(p))
	return 1;
      else if (is_2_face(p))
	return 2;
      else
	return 0;
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
    point2d zfind_root(image2d<point2d>& zpar, point2d x)
    {
      if (zpar(x) == x)
	return x;
      return zpar(x) = zfind_root(zpar, zpar(x));
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
	std::cout << "  nleaves = " << (nleaves_0 + nleaves_1 + nleaves_2)
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
	std::cout << "  nnodes = " << (nnodes_0 + nnodes_1 + nnodes_2)
		  << "\t(#0=" << nnodes_0 << "\t#1=" << nnodes_1 << "\t#2=" << nnodes_2 << ")"
		  << std::endl;
      }


      // About parenthood.
      {
	unsigned n[3][3];
	n[0][0] = n[0][1] = n[0][2] = 0;
	n[1][0] = n[1][1] = n[1][2] = 0;
	n[2][0] = n[2][1] = n[2][2] = 0;
	mln_piter(box2d) p(parent.domain());
	for_all(p)
	  ++n[face_id(p)][face_id(parent(p))];
	std::cout << "  0->0:" << n[0][0] << "  0->1:" << n[0][1] << "  0->2:" << n[0][2] << std::endl;
	std::cout << "  1->0:" << n[1][0] << "  1->1:" << n[1][1] << "  1->2:" << n[1][2] << std::endl;
	std::cout << "  2->0:" << n[2][0] << "  2->1:" << n[2][1] << "  2->2:" << n[2][2] << std::endl;
      }

    }
    


    // -----------------------------------------   attributes



    struct data_t
    {
      unsigned L_var, R_var;
      unsigned L_len, R_area;
      float R_level_sum;
      unsigned R_level_min, R_level_max;

      data_t()
      {
	L_var = R_var = 0;
	L_len = R_area = 0;
	R_level_sum = 0;

	R_level_min = mln_max(unsigned);
	R_level_max = mln_min(unsigned);
      }

      unsigned L_var_mean() const
      {
	if (L_len == 0)
	  return 1;
	return L_var / L_len;
      }

      float R_mean_level() const
      {
	if (R_area == 0)
	  std::abort();
	return R_level_sum / float(R_area);
      }

      void operator+=(const data_t& d)
      {
	L_var += d.L_var;
	R_var += d.R_var;
	L_len += d.L_len;
	R_area += d.R_area;
	R_level_sum += d.R_level_sum;
	if (d.R_level_min < R_level_min) R_level_min = d.R_level_min;
	if (d.R_level_max > R_level_max) R_level_max = d.R_level_max;
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
    depict_R_mean_level(const image2d<point2d>& parent,
			const image2d<R>& K_,
			const image2d<data_t>& dta)
    {
      image2d<value::int_u8> out(parent.domain());
      mln_piter_(box2d) p(parent.domain());
      for_all(p)
	if (parent(p) == p || K_(parent(p)) != K_(p)) // node
	  out(p) = dta(p).R_mean_level();
      for_all(p)
	if (K_(parent(p)) == K_(p)) // not a node
	  out(p) = out(parent(p));
      return out;
    }



    template <typename R>
    image2d<value::int_u8>
    depict_R_level_max(const image2d<point2d>& parent,
			const image2d<R>& K_,
			const image2d<data_t>& dta)
    {
      image2d<value::int_u8> out(parent.domain());
      mln_piter_(box2d) p(parent.domain());
      for_all(p)
	if (parent(p) == p || K_(parent(p)) != K_(p)) // node
	  out(p) = dta(p).R_level_max;
      for_all(p)
	if (K_(parent(p)) == K_(p)) // not a node
	  out(p) = out(parent(p));
      return out;
    }

    template <typename R>
    image2d<value::int_u8>
    depict_R_level_min(const image2d<point2d>& parent,
			const image2d<R>& K_,
			const image2d<data_t>& dta)
    {
      image2d<value::int_u8> out(parent.domain());
      mln_piter_(box2d) p(parent.domain());
      for_all(p)
	if (parent(p) == p || K_(parent(p)) != K_(p)) // node
	  out(p) = dta(p).R_level_min;
      for_all(p)
	if (K_(parent(p)) == K_(p)) // not a node
	  out(p) = out(parent(p));
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
      
      const box2d b = K.domain();
      const unsigned n_faces = b.nsites();


      // Initialisation.
      // ---------------------------------------------

      // K_

      image2d<T> K_(b);

      // S

      std::vector<P> S;
      S.reserve(b.nsites());

      // deja-vu is aux data

      image2d<bool> deja_vu(b);
      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      // q

      utils::dh_queue<P,T> q;
      q.init(l_start, false); // so going down.

      {
	mln_piter(box2d) p(b);
	for_all(p)
	  if (is_frontiere_face(p, b))
	    {
	      q.push(p, K(p));
	      deja_vu(p) = true;
	    }
      }



      //  1st pass: ordering.
      // ---------------------------------------------

      P p;
      mln_niter(N) n(nbh, p);

      while (! q.is_empty())
	{
	  p = q.pop();

	  S.push_back(p);
	  K_(p) = q.level();
	    
	  for_all(n)
	    if (! deja_vu(n))
	      {
		q.push(n, K(n));
		deja_vu(n) = true;
	      }
	}


      //  2nd pass: rough tree.
      // ---------------------------------------------


      image2d<P> parent(b);

      // data for compression and union by rank:
      image2d<P> zpar(b);
      image2d<unsigned> rnk(b), last(b);
      const unsigned not_deja_vu = mln_max(unsigned);
      data::fill(rnk, 0);
      data::fill(last, not_deja_vu);

      // attributes
      image2d<bool> contour(b);
      data::fill(contour, false);
      image2d<data_t> dta(b);

      T l = K_(S[n_faces - 1]);
      for (int i_face = n_faces - 1; i_face >= 0; --i_face)
	{
	  p = S[i_face];

	  if (K_(p) != l)
	    {
	      if (log)
		{
		  std::cout << "contour at " << l;
		  pretty_println(" ", contour);
		}
	      l = K_(p);
	    }

	  parent(p) = p;
	  zpar(p) = p;
	  last(p) = i_face;

	  for_all(n)
	    {
	      if (last(n) == not_deja_vu)
		continue;

	      P r_ = zfind_root(zpar, n),
		p_ = zfind_root(zpar, p);
	      if (r_ != p_)
		{
		  P r = S[last(r_)];
		  parent(r) = p; // p is the root for the uncompressed 'parent' relationship.
		  dta(p) += dta(r);

		  if (rnk(p_) > rnk(r_))
		    {
		      zpar(r_) = p_;
		      if (last(r_) < last(p_))
			last(p_) = last(r_);
		    }
		  else
		    {
		      zpar(p_) = r_;
		      if (last(p_) < last(r_))
			last(r_) = last(p_);
		      if (rnk(p_) == rnk(r_))
			++rnk(r_);
		    }
		}
	    }

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
	      dta(p).R_level_sum += K_(p);

	      if (K_(p) < dta(p).R_level_min) dta(p).R_level_min = K_(p);
	      if (K_(p) > dta(p).R_level_max) dta(p).R_level_max = K_(p);

	      for_all(n)
		if (! contour(n) && last(n) == not_deja_vu)
		  {
		    contour(n) = true;
		    dta(p).L_var += K(n).dist();
		    ++dta(p).L_len;
		  }
	    }
	  
	}



      //  3rd pass: canonized tree.
      // ---------------------------------------------

//       if (log)
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
	  std::cout << std::endl
		    << "after canonization:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}

      for (int i = 0; i < S.size(); ++i)
	{
	  P p = S[i],
	    q = parent(p),
	    r = parent(q);
	  if (is_not_2_face(q))
	    parent(p) = r;
	}    

//       if (log)
	{
	  std::cout << std::endl
		    << "after removing 0- and 1-faces:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}

	unsigned count_parent = 0;
	for (int i = 0; i < S.size(); ++i)
	  {
	    P p = S[i], q = parent(p);
	    if (is_not_2_face(p) || q == p)
	      continue;
	    if (is_not_2_face(q))
	      ++count_parent;
	  }    
	std::cout << "### " << count_parent << std::endl;


      // HERE
// after canonization:
// nleaves = 106   (#0=35  #1=53   #2=18)
// nnodes = 24     (#0=1   #1=7    #2=16)
// after removing 0- and 1-faces:
// nleaves = 113   (#0=35  #1=60   #2=18)
// nnodes = 37     (#0=3   #1=11   #2=23)


//       for (int i = 0; i < S.size(); ++i)
// 	{
// 	  P p = S[i],
// 	    q = parent(p),
// 	    r = parent(q);
// 	  if (is_not_2_face(q))
// 	    parent(p) = r;
// 	}    



      //  Log.
      // ---------------------------------------------


//       io::pgm::save(depict_L_var_mean(parent, K_, dta), "temp_L_var_mean.pgm");

//       io::pgm::save(only_2_faces(depict_R_mean_level(parent, K_, dta)), "temp_R_mean_level.pgm");
//       io::pgm::save(only_2_faces(depict_R_level_max(parent, K_, dta)), "temp_R_level_max.pgm");
//       io::pgm::save(only_2_faces(depict_R_level_min(parent, K_, dta)), "temp_R_level_min.pgm");

//       io::pgm::save(depict_R_mean_level(parent, K_, dta), "temp_R_mean_level_.pgm");
//       io::pgm::save(depict_R_level_max(parent, K_, dta), "temp_R_level_max_.pgm");
//       io::pgm::save(depict_R_level_min(parent, K_, dta), "temp_R_level_min_.pgm");


//       io::pgm::save(K_, "temp_K.pgm");
//       io::pgm::save(gradient(K), "temp_grad.pgm");

      image2d<value::int_u8> G = area_filter(parent, S, K_, dta, lambda);
      io::pgm::save(G, "temp_filtered_.pgm");
      io::pgm::save(only_2_faces(G), "temp_filtered.pgm");

    }




  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda log" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 4)
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

  int_u8 m = frontiere_median(f);
  if (log)
    std::cout << "frontiere median = " << m << std::endl
	      << std::endl;

  image2d<R> K = immerse(f);

  // debug::println("K", K);

  interpolate(K);
  set_frontiere(K, R(m,m));

  // debug::println("K", K);

  compute_autodual_tree(K, m, c4(), lambda, log);
}
