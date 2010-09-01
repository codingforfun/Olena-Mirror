#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>


int main(int argc, char* argv[])
{
  using namespace mln;

  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);
  debug::println("f", f);
}
