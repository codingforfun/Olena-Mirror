
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/lrde/ufmt/spx_maxtree.hh>
#include <oln/lrde/ufmt/log.hh>
#include <oln/lrde/ufmt/utils.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl;
  std::cerr << "max-tree computation with union-find:" << std::endl;
  std::cerr << "  s. sorting for high or no- quant (loop is 'for p')" << std::endl;
  std::cerr << "  p. image<point>" << std::endl;
  std::cerr << "  x. with full compression as aux data" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace oln;
  typedef image2d<ntg::int_u8> image_t;

  image_t raw_input = load(argv[1]);
  image2d<float> input = lrde::ufmt::to_float_with_noise(raw_input);

  int c = atoi(argv[2]);
  if (not (c == 4 or c == 8))
    usage(argv);

  typedef lrde::ufmt::spx_maxtree< image2d<float> > algorithm_t;

  algorithm_t run(input,
		  c == 4 ? neighb_c4() : neighb_c8());
  run.go<void>();
  // save(tree_image(run, true), "tree.pgm");

//   unsigned n = run.nnodes;
//   assert(n == n_level_roots(run));
  std::cout << "n level roots = " << n_level_roots(run) << std::endl;
}
