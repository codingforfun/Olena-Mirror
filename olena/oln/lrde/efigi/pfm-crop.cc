#include <cstdlib>

#include <oln/lrde/efigi/misc.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>

#include <oln/convol/fast_gaussian.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0]
            << " input.pfm fileout.pfm x y dx dy" << std::endl;
  exit(0);
}




int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 7)
    usage(argv);

  assert(sigma > 0);
  image2d<float> input = efigi::load_pfm(argv[1]);
  coord x = atoi(argv[3]);
  coord y = atoi(argv[4]);
  coord dx = atoi(argv[5]);
  coord dy = atoi(argv[6]);
  image2d<float> output = efigi::crop(input, x, y, dx, dy);
  efigi::save_pfm(output, argv[2]);
}
