#include <oln/lrde/efigi/misc.hh>
#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>

#include <oln/morpho/watershed.hh>
#include <ntg/real/int_u.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0]
            << " filein.pfm fileout.pbm" << std::endl;
  exit(0);
}


int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 3)
    usage(argv);

  image2d<float> ima = efigi::load_pfm(argv[1]);
  image2d<unsigned char> ws = morpho::watershed_seg<unsigned char>(ima, neighb_c8());
  save(efigi::lab2bin(ws), argv[2]);
}
