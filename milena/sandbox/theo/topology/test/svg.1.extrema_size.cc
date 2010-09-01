#include <map>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pgm/load.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/flat_zones.hh>

#include <mln/labeling/compute.hh>
#include <mln/morpho/attribute/card.hh>



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
  typedef std::map<unsigned, unsigned> map_t;

  {
    image2d<unsigned> labels = labeling::regional_minima(in,
							 c == 4 ? c4() : c8(),
							 n);
    std::cout << "  regmins: #" << n << "  min area = ";

    morpho::attribute::card<I> a;
    util::array<unsigned> area = labeling::compute(a, labels, n);
    map_t m;
    for (unsigned i = 1; i <= n; ++i)
      ++m[area[i]];
    unsigned max_area = 0;
    for (map_t::const_iterator i = m.begin(); i != m.end(); ++i)
      {
	unsigned a = i->first;
	if (a > max_area)
	  max_area = a;
	std::cout << a << "(#" << i->second << ") ";
      }
    std::cout << std::endl
	      << "  max area = " << max_area << std::endl;
  }

  {
    image2d<unsigned> labels = labeling::regional_maxima(in,
							 c == 4 ? c4() : c8(),
							 n);
    std::cout << "  regmaxs: #" << n << "  ->  ";

    morpho::attribute::card<I> a;
    util::array<unsigned> area = labeling::compute(a, labels, n);
    map_t m;
    for (unsigned i = 1; i <= n; ++i)
      ++m[area[i]];
    unsigned max_area = 0;
    for (map_t::const_iterator i = m.begin(); i != m.end(); ++i)
      {
	unsigned a = i->first;
	if (a > max_area)
	  max_area = a;
	std::cout << a << "(#" << i->second << ") ";
      }
    std::cout << std::endl
	      << "  max area = " << max_area << std::endl;
  }

  {
    image2d<unsigned> labels = labeling::flat_zones(in,
						    c == 4 ? c4() : c8(),
						    n);
    std::cout << "  flatzs: #" << n << "  ->  ";

    morpho::attribute::card<I> a;
    util::array<unsigned> area = labeling::compute(a, labels, n);
    map_t m;
    for (unsigned i = 1; i <= n; ++i)
      ++m[area[i]];
    unsigned max_area = 0;
    for (map_t::const_iterator i = m.begin(); i != m.end(); ++i)
      {
	unsigned a = i->first;
	if (a > max_area)
	  max_area = a;
	std::cout << a << "(#" << i->second << ") ";
      }
    std::cout << std::endl
	      << "  max area = " << max_area << std::endl;
  }

}
