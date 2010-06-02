#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>
#include <mln/histo/compute.hh>
#include <mln/io/pgm/load.hh>
#include <fstream>

#ifndef Q
# define Q int_u12
#endif

void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0] << " ima.pgm" << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::int_u16;
  typedef Q V;

  if (argc < 2)
    usage(argv);
  const char* finput = argv[1];
  typedef image2d<V> I;

  I input;
  io::pgm::load(input, finput);

  histo::array<V> h = histo::compute(input);

  std::ofstream fo("histo.cvs");
  int nvalues = mln_card(V);
  int c = 0;
  for (int i = 0; i < nvalues; ++i)
    {
      if (h[i] != 0)
	++c;
      fo << i << ", " << h[i] << std::endl;
    }

  fo.close();
}
