#include <cstdlib>

#include <ntg/int.hh>
#include <ntg/color/rgb.hh>

#include <oln/basics2d.hh>
#include <oln/level/fill.hh>

// Max-tree computation base on Fiorio's and Gustedt's algorithm.
#if defined(FIORIO_VERSION) && FIORIO_VERSION == 2
# include <oln/lrde/ufmt/fiorio-2.hh>
#else
# include <oln/lrde/ufmt/fiorio-1.hh>
#endif


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm"
	    << std::endl
	    << "Max-tree computation with Fiorio's and Gustedt's algorithm."
	    << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  typedef oln::image2d<ntg::int_u8> image_type;
  image_type input = oln::io::load(argv[1]);

  typedef oln::lrde::ufmt::fiorio<image_type> algorithm_type;
  algorithm_type run(input);
  std::cout << "npoint                        = " << run.ima().npoints()
	    << std::endl;
  run.go();
  std::cout << "n level roots (first method)  = " << run.n_level_roots1()
	    << std::endl
	    << "n level roots (second method) = " << run.n_level_roots2()
	    << std::endl;
}
