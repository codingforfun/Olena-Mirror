
#include <cstdlib>

#include <ntg/int.hh>
#include <oln/basics1d.hh>
#include <oln/lrde/ufmt/r1ic_maxtree.hh>
#include <oln/lrde/ufmt/log.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input1D.pgm" << std::endl;
  std::cerr << "1D raster max-tree computation with union-find" << std::endl;
  std::cerr << "  r. for raster video scan" << std::endl;
  std::cerr << "  i. par = image<int>" << std::endl;
  std::cerr << "  c. level compression" << std::endl;
  std::cerr << "inspecting the only neighbor calls 'insert'" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  using namespace oln;
  typedef image1d<ntg::int_u8> image_t;

  image_t input = load(argv[1]);
  typedef lrde::ufmt::r1ic_maxtree<image_t> algorithm_t;

  algorithm_t run(input);
  run.go();
  std::cout << "n level roots = " << n_level_roots(run) << std::endl;
}
