
#include <vector>
#include <cmath>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
#include <mln/extension/adjust.hh>
#include <mln/extension/fill.hh>

#include <mln/util/timer.hh>

#include "../utils/dh_queue.hh"
#include "../utils/h_median.hh"
#include "../utils/frontiere.hh"




namespace mln
{

  namespace k4
  {


    //      0 1 2 
    //
    //  0   a b c
    //  1   d e f


    //   becomes:


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
	  d(p) = K(p).variation();
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


    template <typename I>
    inline
    mln_site(I)
    zfind_root(I& zpar, mln_site(I) x)
    {
      if (zpar(x) == x)
	return x;
      return zpar(x) = zfind_root(zpar, zpar(x));
    }


    template <typename P, typename T>
    void
    print_tree_stats(const image2d<P>& parent,
		     const std::vector<P>& S,
		     const image2d<T>& K_)
    {

      // About leaves.
      {
	P p;
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
	      if (k4::is_0_face(p))
		++nnodes_0;
	      else if (k4::is_1_face(p))
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
	  ++n[k4::face_id(p)][k4::face_id(parent(p))];
	std::cout << "  0->0:" << n[0][0] << "  0->1:" << n[0][1] << "  0->2:" << n[0][2] << std::endl;
	std::cout << "  1->0:" << n[1][0] << "  1->1:" << n[1][1] << "  1->2:" << n[1][2] << std::endl;
	std::cout << "  2->0:" << n[2][0] << "  2->1:" << n[2][1] << "  2->2:" << n[2][2] << std::endl;
      }

    }
    


    template <typename P, typename T, typename A>
    void
    back_propagate_inplace(const image2d<P>& parent,
			   const std::vector<P>& S,
			   const image2d<T>& f,
			   image2d<A>& a)
    {
      for (int i = 1; i < S.size(); ++i) // from ancestors to children
	{
	  P p = S[i], q = parent(p);
	  if (f(q) == f(p)) // p is not a node so...
	    {
	      a(p) = a(q);    // ...propagate from q to its child p

	      mln_invariant(parent(q) == q || f(parent(q)) != f(q)); // q is a node
	    }
	}
    }



    template <typename P, typename T, typename A>
    image2d<T>
    filter(const image2d<P>& parent,
	   const std::vector<P>& S,
	   const image2d<T>& f,
	   const image2d<A>& a,
	   A lambda)
    {
      image2d<T> g(f.domain());

      // root
      {
	P p = S[0];
	g(p) = f(p);
      }

      for (int i = 1; i < S.size(); ++i)
	{
	  P p = S[i], q = parent(p);
	  // there is no another root:
	  mln_invariant(q != p);

	  if (f(q) != f(p)) // p is a node
	    {
	      // a is growing:
	      mln_invariant(a(q) > a(p));
	      
	      if (a(p) >= lambda)
		g(p) = f(p);
	      else
		g(p) = g(q);
	    }
	  else // same as its node
	    {
	      // q is a node:
	      mln_invariant(parent(q) == q || f(parent(q)) != f(q));

	      g(p) = g(q);
	    }
	}

      return g;
    }



    // -----------------------------------------   log



    template <typename I>
    mln_ch_value(I, value::int_u8)
      log_(const Image<I>& a_)
    {
      const I& a = exact(a_);
      mln_ch_value(I, float) log_f;
      initialize(log_f, a);
      mln_piter(I) p(a.domain());
      for_all(p)
	log_f(p) = std::log(1.f + float(a(p)));
      return data::stretch(value::int_u8(), log_f);
    }



    // -----------------------------------------   tree computation



    template <typename T, typename N, typename A>
    image2d<T>
    compute_autodual_tree(image2d<T>& f, const N& nbh, A lambda,
			  bool log = false)
    {
      typedef utils::range<T> R;
      typedef point2d P;
      
      extension::adjust(f, nbh);

      util::array<int> dp = offsets_wrt(f, nbh);
      const unsigned n_nbhs = dp.nelements();



      // Initialisation.
      // ---------------------------------------------

      // l_start

      bool start_up;
      T l_start = utils::frontiere_median(f, start_up);
      if (log)
	std::cout << "l_start = " << l_start << "  "
		  << "start_up = " << (start_up ? "true" : "false") << std::endl;

      // K

      image2d<R> K = immerse(f);
      interpolate(K); // FIXME: HERE

      const box2d b = K.domain();
      const unsigned n_faces = b.nsites();

      // deja-vu (aux data)

      image2d<bool> deja_vu(b);
      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      // q

      utils::dh_queue<P,T> q;
      q.init(l_start, start_up);
      utils::initialize_frontiere(K, q, deja_vu);

      if (log)
	debug::println("K", K);

      // K_

      image2d<T> K_(b);
      data::fill(K_, 0); // only for debug

      // S

      std::vector<P> S;
      S.reserve(b.nsites());


      // debug data ('ordering' and 'in_queue')

      image2d<unsigned> ordering(b);
      unsigned index = 0;

      image2d<bool> in_queue = duplicate(deja_vu);
      image2d<bool> done(b);
      data::fill(done, false);



      //  1st pass: ordering.
      // ---------------------------------------------

      P p;
      mln_niter(N) n(nbh, p);

      while (! q.is_empty())
	{
	  p = q.pop();
	  S.push_back(p);
	  K_(p) = q.level();

	  in_queue(p) = false;    // debug
	  ordering(p) = ++index;  // debug

	    
	  for_all(n)
	    if (! deja_vu(n))
	      {
		q.push(n, K(n));
		deja_vu(n) = true;
		in_queue(n) = true;  // debug
	      }

	  done(p) = true;  // debug

	  if (log && q.is_level_completed())
	    {
	      std::cout << std::endl
			<< "level = " << q.level() << std::endl;
	      pretty_println("done:",     done);
	      pretty_println("in-queue:", in_queue);
	    }


	  // FIXME: add an invariant about deja_vu = done + in_queue
	}


      if (log)
	{
	  debug::println("K_", K_);
	  debug::println("S", ordering);
	}



      //  2nd pass: rough tree.
      // ---------------------------------------------


      image2d<P> parent(b);

      // data for compression and union by rank:
      image2d<P> zpar(b);
      P null(-1,-1);
      data::fill(zpar, null);
      image2d<unsigned> rnk(b), last(b);
      data::fill(rnk, 0);


      // topological objects

      image2d<bool> contour(b);
      data::fill(contour, false);


      // attributes
      image2d<unsigned> area(b), perimeter(b), tv(b), lv(b);
      data::fill(area, 0);
      data::fill(perimeter, 0);
      data::fill(tv, 0);
      data::fill(lv, 0); // lv is the contour variation

      data::fill(deja_vu, false);  // debug
      unsigned n_trees = 0;        // debug
      T l = K_(S[n_faces - 1]);

      for (int i = n_faces - 1; i >= 0; --i)
	{
	  p = S[i];

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
	  last(p) = i;
	  ++n_trees;  // debug
	  
	  for_all(n)
	  {
	    if (zpar(n) == null)
	      continue;

	    P r_ = zfind_root(zpar, n),
	      p_ = zfind_root(zpar, p);
	    if (r_ != p_)
	      {
		P r = S[last(r_)];
		parent(r) = p; // p is the root for the uncompressed 'parent' relationship.
		--n_trees;  // debug

		area(p) += area(r);
		perimeter(p) += perimeter(r);
		tv(p) += tv(r);
		lv(p) += lv(r);

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
	  
	  deja_vu(p) = true;  // debug

	  if (is_0_face(p))
	    {
	      if (contour(p))
		contour(p) = false;
	    }
	  else if (is_1_face(p))
	    {
	      if (contour(p))
		{
		  // exterior contour is now interior
		  contour(p) = false;
		  perimeter(p) -= 1;
		  lv(p) -= K(p).variation();
		  tv(p) += K(p).variation();
		} 
	    }
	  else if (is_2_face(p))
	    {
 	      area(p) += 1;
	      for_all(n)
		if (! contour(n) && zpar(n) == null)
		  {
		    contour(n) = true;
		    perimeter(p) += 1;
		    lv(p) += K(n).variation();
		  }

	      mln_niter(N) n_(c8(), p);
	      for_all(n_)
		if (! contour(n_) && zpar(n_) == null)
		  contour(n_) = true;
	    }
	  
	}



      //  3rd pass: canonized tree.
      // ---------------------------------------------


      if (log)
	{
	  std::cout << std::endl
		    << "before canonization:" << std::endl;
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

      if (log)
	{
	  std::cout << std::endl
		    << "after canonization:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}




      //  Log.
      // ---------------------------------------------

      if (log)
	{
	  io::pgm::save(K_, "temp_K_.pgm");
	  io::pgm::save(gradient(K), "temp_grad.pgm");


	  back_propagate_inplace(parent, S, K_, area);
	  back_propagate_inplace(parent, S, K_, perimeter);
	  back_propagate_inplace(parent, S, K_, tv);
	  back_propagate_inplace(parent, S, K_, lv);


	  io::pgm::save(log_(area), "temp_area.pgm");
	  io::pgm::save(log_(perimeter), "temp_perimeter.pgm");
	  io::pgm::save(log_(tv), "temp_tv.pgm");
	  io::pgm::save(log_(lv), "temp_lv.pgm");
	}


      //  Filtering.
      // ---------------------------------------------


      image2d<T> G = filter(parent, S, K_, area, lambda);

      if (log)
	io::pgm::save(G, "temp_G.pgm");


      return only_2_faces(G);
    }



  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm [log]" << std::endl
	    << "  log = 0 or 1 (optional)" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 4 && argc != 5)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  using value::int_u8;
  typedef utils::range<int_u8> R;

  image2d<int_u8> f, g;
  io::pgm::load(f, argv[1]);

  unsigned lambda = std::atoi(argv[2]);
  bool log = std::atoi(argv[argc - 1]);

  if (log)
    debug::println("f", f);

  g = compute_autodual_tree(f, c4(), lambda, log);

  if (log)
    debug::println("g", g);

  io::pgm::save(g, argv[3]);
}
