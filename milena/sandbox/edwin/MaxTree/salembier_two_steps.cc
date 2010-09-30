#include <mln/io/pgm/load.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/data/fill.hh>
#include <mln/extension/fill.hh>

#include <mln/util/hqueues.hh>
#include <mln/util/array.hh>
#include <mln/debug/println.hh>

namespace mln
{
  typedef image2d< value::int_u8 > I;

  struct salembier_args
  {
    typedef unsigned P;
    typedef value::int_u8 V;

    enum { nvalues = mln_card(V) };

    const I&					f;
    util::array<int>				dp;
    unsigned					n_nbhs;

    unsigned					hmin;
    util::hqueues<P, V>				hqueue;
    P						levroot[nvalues];
    bool					is_node_at_level[nvalues];
    image2d<unsigned>				parent;
    image2d<bool>				deja_vu;
    std::vector<P>				porder;
  };

  int flood(salembier_args& args, const int h)
  {
    typedef unsigned P;

    mln_assertion(args.is_node_at_level[h]);

    P root = args.levroot[h];

    while (!args.hqueue[h].empty())
      {
	P p = args.hqueue[h].pop_front();
	args.parent.element(p) = root;

	// Process the neighbors
	for (unsigned j = 0; j < args.n_nbhs; ++j)
	  {
	    unsigned n = p + args.dp[j];

	    if (!args.deja_vu.element(n))
	    {
	      int n_idx = args.f.element(n); // en faite vset.index_of(...)
	      args.hqueue[n_idx].push(n);
	      args.deja_vu.element(n) = true;

	      if (!args.is_node_at_level[n_idx])
		{
		  args.levroot[n_idx] = n;
		  args.is_node_at_level[n_idx] = true;
		}

	      while (h < n_idx)
		n_idx = flood(args, n_idx);
	    }
	  }
	args.porder.push_back(p);
      }

    args.is_node_at_level[h] = false;
    unsigned c = h;
    while (c > args.hmin && !args.is_node_at_level[c])
      --c;

    if (args.is_node_at_level[c])
      {
	P q = args.levroot[c];
	args.parent.element(root) = q;
	//args.count_edges(q) += args.count_edges(root); // attribute.merge
      }
    else
      args.parent.element(root) = root;

    return c;
  }

  salembier_args
  salembier(const image2d<value::int_u8>& f, const neighb2d& nbh = c4())
  {
    // Init
    enum { nv = mln_card(mln_value_(I)) };

    salembier_args args = { f};
    {
      box2d b(2 * f.nrows() - 1, 2 * f.ncols() - 1);

      args.dp = offsets_wrt(f, nbh);
      args.n_nbhs = args.dp.nelements();

      args.porder.reserve(f.nsites());
      initialize(args.parent, f);
      initialize(args.deja_vu, f);
      //initialize(args.count_edges, f);
      //args.active_edges.init_(b);

      std::fill(args.is_node_at_level, args.is_node_at_level + nv, false);
      data::fill(args.deja_vu, false);
      extension::fill(args.deja_vu, true);
      //data::fill(args.active_edges, false);
      //data::fill(args.count_edges, 0);
    }

    // Get start value
    unsigned pstart =  f.index_of_point(f.domain().pmin());
    mln_value_(I) hstart = f.element(pstart);
    {
      unsigned h[nv] = {0};
      mln_pixter_(const I) px(f);
      for_all(px)
      {
	mln_value_(I) v = px.val();
	++h[v];
	if (v < hstart)
	  {
	    hstart = v;
	    pstart = px.offset();
	  }
      }

      for (unsigned i = 0; i < nv; ++i)
	args.hqueue[i].reserve(h[i]);
    }

    // Start flooding
    args.hmin = hstart;
    args.deja_vu.element(pstart) = true;
    args.hqueue[hstart].push(pstart);
    args.levroot[hstart] = pstart;
    args.is_node_at_level[hstart] = true;

    int r = flood(args, hstart);
    mln_assertion(r == hstart);
    return args;
  }

  unsigned zfindroot(image2d<unsigned>& parent, unsigned p)
  {
    if (parent.element(p) == p)
      return p;
    else
      return (parent.element(p) = zfindroot(parent, parent.element(p)));
  }


  image2d<unsigned>
  unionfind(const image2d<value::int_u8>& ima, const neighb2d& nbh,
	    const std::vector<unsigned>& s)
  {
    image2d<unsigned> parent;
    image2d<unsigned> zpar;

    initialize(parent, ima);
    initialize(zpar, ima);
    data::fill(parent, 0);

    util::array<int> dp = offsets_wrt(ima, nbh);
    unsigned n_nbhs = dp.nelements();

    int nsites = s.size();
    for (int i = 0; i < nsites; ++i)
      {
	unsigned p = s[i];
	parent.element(p) = p; // make set
	zpar.element(p) = p; // make set

	for (int j = 0; j < n_nbhs; ++j)
	  {
	    unsigned n = p + dp[j];
	    if (parent.element(n) != 0) // deja vu
	      { // make union
		unsigned r = zfindroot(zpar, n);
		zpar.element(r) = p;
	      }
	  }
      }

    // canonisation
    for (int i = nsites - 1; i >=0; --i)
      {
	unsigned p = s[i];
	unsigned q = parent.element(q);
	if (q == parent.element(q))
	  parent.element(p) = parent.element(q);
      }
    return parent;
  }

}

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.png" << std::endl;
  std::cout << "Calcule un MaxTree avec UnionFind mix Salembier ordering"
	    << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  using namespace mln;
  typedef image2d< value::int_u8 > I;

  if (argc < 2)
    usage(argv);

  const char* filename = argv[1];

  I input;
  io::pgm::load(input, filename);

  salembier_args a = salembier(input, c4());

  image2d<unsigned> parent = unionfind(input, c4(), a.porder);

  std::cout << "Attribute Image: " << std::endl;
  debug::println(a.parent);
  debug::println(a.parent);
}
