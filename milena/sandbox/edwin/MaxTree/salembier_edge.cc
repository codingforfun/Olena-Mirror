#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>
#include "salembier_edge.hh"

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.png" << std::endl;
  std::cout << "Calcule la longueur des contours des CC avec l'algo de Salembier."
	    << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  using namespace mln;
  typedef image2d< value::int_u8 > I;

  if (argc < 2)
    usage(argv);

  const char* filename = argv[1];

  I input;
  io::pgm::load(input, filename);

  image2d<int> e = salembier(input);

  std::cout << "Attribute Image: " << std::endl;
  debug::println(e);
}
