#include <cstdlib>

#include <oln/lrde/efigi/misc.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>

#include <oln/convol/fast_gaussian.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0]
            << " filein.pgm sigma fileout.pfm" << std::endl;
  exit(0);
}




int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 4)
    usage(argv);

  float sigma = std::atof(argv[2]);
  assert(sigma > 0);
  image2d<float> ima = efigi::load_pfm(argv[1]);
  image2d<float> imaG = convol::fast::gaussian(ima, sigma);
  efigi::save_pfm(imaG, argv[3]);
}
