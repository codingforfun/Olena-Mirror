#include <oln/lrde/efigi/misc.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0]
            << " filein.pgm fileout.pfm" << std::endl;
  exit(0);
}



int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 3)
    usage(argv);

  image2d<unsigned char> ima = oln::load(argv[1]);
  image2d<float> imaf = efigi::add_float_noise(ima);
  efigi::save_pfm(imaf, argv[2]);
}
