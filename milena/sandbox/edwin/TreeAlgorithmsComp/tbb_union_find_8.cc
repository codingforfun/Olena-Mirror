#undef MLN_WO_GLOBAL_VARS

#include <tbb/task_scheduler_init.h>

#include "parallel_union_find.hh"

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include "nnodes.hh"

#undef BENCH_START
#undef BENCH_END
#include "bench.hh"

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm nbExec [nbThread]" << std::endl;
  std::cerr << "  max-tree parallel" << std::endl;
  std::abort();
}

int main(int argc, char* argv[])
{
  if (argc < 3)
    usage(argv);

  int nthread = (argc == 4) ? std::atoi(argv[3]) : tbb::task_scheduler_init::automatic;
  tbb::task_scheduler_init init(nthread);

  using namespace mln;
  neighb2d nbh = c4();
  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);

  image2d<unsigned> parent = compute_max_tree(f, nbh);
  std::cout << "nnodes = " << nnodes(f, parent) << std::endl;

  START_BENCH(std::atoi(argv[2]));
  parent = compute_max_tree(f, nbh);
  END_BENCH("Parallel union find: ");
}

