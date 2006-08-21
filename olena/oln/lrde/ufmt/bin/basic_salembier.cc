
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/lrde/ufmt/basic_salembier.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl;
  std::cerr << "basic max-tree computation with salembier's flooding" << std::endl;
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
  if (not (c == 2 or c == 4 or c == 8))
    usage(argv);

  typedef lrde::ufmt::basic_salembier<image_t> algorithm_t;

  neighborhood2d nbh;
  if (c == 2)
    nbh.add(dpoint2d(0, 1));
  else
    nbh = c == 4 ? neighb_c4() : neighb_c8();

  algorithm_t run(input, nbh);
  run.go();
  std::cout << "n level roots = " << run.n_level_roots() << std::endl;
}
