#include <tbb/task_scheduler_init.h>
#include "parallel_union_find.hh"

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

 namespace mln
 {

   template <typename I, typename J>
   unsigned nnodes(const I& f, const J& parent)
   {
     mlc_equal(mln_value(J), unsigned)::check();
     unsigned n = 0;
     mln_piter(I) p(parent.domain());
     for_all(p)
     {
       unsigned o = parent.index_of_point(p);
       if (parent(p) == o || f.element(parent(p)) != f(p))
	 ++n;
     }
     return n;
   }

 }

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "  max-tree parallel" << std::endl;
  std::abort();
}

int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  tbb::task_scheduler_init init;
  using namespace mln;

  neighb2d nbh = c4();
  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);

  image2d<unsigned> parent = compute_max_tree(f, nbh);
  //  debug::println(parent);

  // tm.start();
  // START_BENCH(1);
  // //compute_max_tree(f, nbh);
  // END_BENCH();
  std::cout << "nnodes = " << nnodes(f, parent) << std::endl;
}

