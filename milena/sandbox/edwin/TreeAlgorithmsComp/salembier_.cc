#include <mln/core/image/image2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>

#include <mln/io/pgm/load.hh>

#include "salembier.hh"
#include "nnodes.hh"
#include "bench.hh"

#ifndef Q
# define Q int_u8
#endif

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.pgm nBench" << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  if (argc != 3)
    usage(argv);

  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::int_u16;
  typedef Q V;

  neighb2d nbh = c4();
  image2d<V> input;
  io::pgm::load(input, argv[1]);

  salembier<V> s(input, nbh);
  image2d<unsigned> parent = s.parent;
  std::cout << "nnodes = " << nnodes(input, parent) << std::endl;

  int nb_bench = atoi(argv[2]);
  START_BENCH(nb_bench);
  salembier<V> s(input, nbh);
  parent = s.parent;
  END_BENCH("Salembier: ");
}




