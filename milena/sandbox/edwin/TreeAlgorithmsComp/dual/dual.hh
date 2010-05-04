#ifndef DUAL_HH_
# define DUAL_HH_

#include <mln/border/all.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/dual_input_tree.hh>
#include <mln/core/image/attribute_image.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/ctree.hh>
#include <mln/io/pgm/save.hh>

using namespace mln;

typedef value::int_u8 V;
typedef image2d<V> I;
typedef util::ctree::ctree<I> tree_t;
typedef morpho::attribute::card<I> attr_t;
typedef mln_result_(attr_t) S;
typedef attribute_image<tree_t, S > A;
typedef neighb2d N;

using value::int_u8;


void
inline
test(const int_u8* f_, const int_u8* m_, int n)
{
  box<point2d> domain(point2d(0, 0), point2d(0, n-1));
  image2d<int_u8> f(domain), m(domain);
  N nbh = c4();
  for (int i = 0; i < n ; ++i)
  {
    f.at_(0, i) = f_[i];
    m.at_(0, i) = m_[i];
  }

  io::pgm::save(f, "input.pgm");
  io::pgm::save(m, "mask.pgm");

  tree_t tree = morpho::tree::dual_input_max_tree(f, m, nbh);
  std::cout << "Volume info " << "" << std::endl
	    << "\tSize: " << geom::nsites(f) << std::endl
	    << "\tQuant: " << sizeof(V) * 4 << std::endl
	    << "\tNodes: " << tree.n_nodes() << std::endl;

  debug::println(tree);
}

#endif
