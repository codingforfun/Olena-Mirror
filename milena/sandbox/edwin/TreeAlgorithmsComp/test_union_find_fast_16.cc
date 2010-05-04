#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u16.hh>
#include <mln/io/pgm/load.hh>
#include <mln/util/timer.hh>

#include <mln/morpho/tree/component_tree.hh>


static mln::util::timer tm;

#define START_BENCH(N_ITERATION)		\
  {						\
  tm.restart();					\
  const int n_iteration__ = N_ITERATION;	\
  for (int i__ = 0; i__ < n_iteration__; ++i__) {


#define END_BENCH(MSG)					\
  }							\
    std::cout << MSG					\
    << ((float)(tm.stop()) * 1000.0 / n_iteration__)	\
    << " ms" << std::endl;				\
    }


void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " image" << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  using namespace mln;
  typedef value::int_u16 V;
  typedef image2d<V> I;
  typedef util::ctree::ctree<I> tree_t;
  typedef neighb2d N;

  I input;
  N nbh = c4();

  if (argc < 2)
    usage(argv);

  const char* finput = argv[1];

  tm.start();
  io::pgm::load(input, finput);

  trace::quiet = false;
  tree_t tree =
    morpho::tree::impl::generic::union_find_fast(tag::tree::max, input, nbh);
  trace::quiet = true;

  std::cout << "Number of nodes: " << tree.n_nodes() << std::endl;

  START_BENCH(10);
  morpho::tree::impl::generic::union_find_fast(tag::tree::max, input, nbh);
  END_BENCH("Union find fast 16bits: ");
}
