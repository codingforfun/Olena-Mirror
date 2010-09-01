#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>

#include "kk2d.hh"


int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::kk2d;

  image2d<int> f(2, 3);
  debug::iota(f);
  debug::println("f", f);

  image2d<int> K = immerge(f);
  debug::println("K", K);

  data::fill((K | is_1_face).rw(), 9);
  debug::println("K", K);

  data::fill((K | is_0_face).rw(), 8);
  debug::println("K", K);

  test_nbh("browsing 2-faces:", f, is_2_face, nbh_2_face());

  test_nbh("browsing 2 -> 1:", f, is_2_face, nbh_2_to_1());
  test_nbh("browsing 1 -> 2:", f, is_1_face, nbh_1_to_2());

  test_nbh("browsing 1-faces:", f, is_1_face, nbh_1_face());


  test_nbh("browsing 0 -> 1:", f, is_0_face, nbh_0_to_1());
}
