
#ifndef UTILS_TREE_HH
# define UTILS_TREE_HH

# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>


namespace mln
{

  // Fwd decl.


  namespace k4
  {
    
    extern bool is_1_face(const point2d& p);
    extern bool is_0_face(const point2d& p);
    extern unsigned face_id(const point2d& p);
  }


  namespace utils
  {


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
	unsigned p = S[0];
	g(p) = f(p);
      }

      for (int i = 1; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent(p);
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



  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_TREE_HH
