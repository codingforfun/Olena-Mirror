
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/lrde/ufmt/hpx_maxtree.hh>
#include <oln/lrde/ufmt/log.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl;
  std::cerr << "max-tree computation with union-find:" << std::endl;
  std::cerr << "  h. for low quant (for h, for p at h)" << std::endl;
  std::cerr << "  p. image<point>" << std::endl;
  std::cerr << "  x. level compression + full compression aux data" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace oln;
  typedef image2d<ntg::int_u8> image_t;

  image_t input = load(argv[1]);

  int c = atoi(argv[2]);
  if (not (c == 4 or c == 8))
    usage(argv);

  typedef lrde::ufmt::hpx_maxtree<image_t> algorithm_t;

  algorithm_t run(input,
		  c == 4 ? neighb_c4() : neighb_c8());
  run.go<void>();
  // save(tree_image(run, true), "tree.pgm");

  unsigned n = run.nnodes;
  assert(n == n_level_roots(run));
  std::cout << "n level roots = " << n << std::endl;
}
