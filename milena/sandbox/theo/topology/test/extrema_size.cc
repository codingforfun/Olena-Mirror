#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/flat_zones.hh>

#include <mln/labeling/compute.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/pw/all.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm c" << std::endl
	    << "   depict area of regional-min/max and flat-zoness" << std::endl
	    << "   c = 4 or 8" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  I in;
  io::pgm::load(in, argv[1]);

  unsigned c = std::atoi(argv[2]);
  if (c != 4 && c != 8)
    usage(argv);

  std::cout << "info c" << c << " on " << argv[1] << ":" << std::endl;

  unsigned n;

  {
    image2d<unsigned> labels = labeling::regional_minima(in,
							 c == 4 ? c4() : c8(),
							 n);
    io::pbm::save((pw::value(labels) != pw::cst(0)) | labels.domain(),
		  "temp_regmin.pbm");

    std::cout << "  regmins: #" << n;

    morpho::attribute::card<I> a;
    util::array<unsigned> area = labeling::compute(a, labels, n);

    unsigned min_area = mln_max(unsigned);
    for (unsigned i = 1; i <= n; ++i)
      if (area[i] < min_area)
	min_area = area[i];
    std::cout << "  min area = " << min_area << std::endl;
  }

  {
    image2d<unsigned> labels = labeling::regional_maxima(in,
							 c == 4 ? c4() : c8(),
							 n);
    io::pbm::save((pw::value(labels) != pw::cst(0)) | labels.domain(),
		  "temp_regmax.pbm");

    std::cout << "  regmaxs: #" << n;

    morpho::attribute::card<I> a;
    util::array<unsigned> area = labeling::compute(a, labels, n);

    unsigned min_area = mln_max(unsigned);
    for (unsigned i = 1; i <= n; ++i)
      if (area[i] < min_area)
	min_area = area[i];
    std::cout << "  min area = " << min_area << std::endl;
  }

  {
    image2d<unsigned> labels = labeling::flat_zones(in,
						    c == 4 ? c4() : c8(),
						    n);
    std::cout << "  flatzns: #" << n << std::endl;
  }

}
