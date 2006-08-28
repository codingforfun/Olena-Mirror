
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/lrde/ufmt/basic_najman.hh>
#include <oln/lrde/ufmt/log.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl;
  std::cerr << "basic max-tree computation with union-find" << std::endl;
  // FIXME: get precise description...
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

  typedef lrde::ufmt::basic_najman<image_t> algorithm_t;

  algorithm_t run(input,
		  c == 4 ? neighb_c4() : neighb_c8());
  run.go();

  //std::cout << "root = " << run.Root << std::endl;
  std::cout << "n level roots = " << n_level_roots(run) << std::endl;
}
