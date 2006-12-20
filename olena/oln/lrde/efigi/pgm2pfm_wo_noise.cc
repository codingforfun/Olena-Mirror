#include <oln/lrde/efigi/io.hh>
#include <oln/lrde/efigi/req.hh>


void usage(char* argv[])
{
  std::cerr << "Convert a PGM to a PFM image without adding noise."
	    << std::endl
	    << "usage: " << argv[0]
            << " filein.pgm fileout.pfm" << std::endl;
  exit(0);
}


int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 3)
    usage(argv);

  image2d<unsigned char> ima = oln::load(argv[1]);
  image2d<float> imaf (ima.size());
  oln_iter_type_(image2d<unsigned char>) p(ima);
  for_all(p)
    imaf[p] = ima[p];
  efigi::save_pfm(imaf, argv[2]);
}
