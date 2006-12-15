#include <oln/lrde/efigi/misc.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0]
            << " filein.pfm fileout.pgm" << std::endl;
  exit(0);
}


int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 3)
    usage(argv);

  image2d<float> ima = efigi::load_pfm(argv[1]);
  float min, max;
  efigi::min_max(ima, min, max);
  image2d<unsigned char> ima8 = efigi::convert_linear_8(ima, min, max);
  save(ima8, argv[2]);
}
