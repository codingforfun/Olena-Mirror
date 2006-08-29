
#include <cstdlib>
#include <oln/basics2d.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " size output.pgm" << std::endl;
  std::cerr << "generate worst-case for salembier (Cf. najman)" << std::endl;
  exit(1);
}

namespace oln {

  template <typename I>
  void salembier_worstcase_image(abstract::image<I>& f)
  {
    oln_iter_type(I) p(f);
    unsigned char c = 0;
    for_all(p)
      if ((p.row() + p.col()) % 2)
	f[p] = 0;
      else
	f[p] = ++c;
  }

}

// 1000   0.71    498048  0.28
// 2000   3.15   1992189  1.37
// 3000   7.26   4482423  3.17
// 4000  15.20   7968751  5.91
// 5000  21.80  12451173  9.42


int main(int argc, char* argv[])
{
  using namespace oln;

  if (argc != 3)
    usage(argv);

  int size = atoi(argv[1]);
  if (size < 0)
    usage(argv);

  image2d<ntg::int_u8> f(size, size);
  salembier_worstcase_image(f);

  save(f, argv[2]);
}
