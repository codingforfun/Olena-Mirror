
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/timer.hh>

#include <oln/lrde/ufmt/generic_salembier.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl;
  std::cerr << "basic max-tree computation with salembier's flooding"
	    << std::endl
	    << "on a float image, using a 16-bit equalization."
	    << std::endl;
  // FIXME: get precise description...
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace oln;
  typedef image2d<unsigned short> image_t;

  image2d<float> input_float = efigi::load_pfm(argv[1]);
  image_t input = efigi::equalize_16(input_float);
  assert (input.has_impl());

  int c = atoi(argv[2]);
  if (not (c == 2 or c == 4 or c == 8))
    usage(argv);

  typedef lrde::ufmt::generic_salembier<image_t> algorithm_t;

  neighborhood2d nbh;
  if (c == 2)
    nbh.add(dpoint2d(0, 1));
  else
    nbh = c == 4 ? neighb_c4() : neighb_c8();

  algorithm_t run(input, nbh);
  utils::timer t;
  t.start();
  run.go();
  t.stop();
  std::cout << "n level roots = " << run.n_level_roots() << std::endl
	    << "elapsed time  = " << t.last_time() << std::endl;
}
