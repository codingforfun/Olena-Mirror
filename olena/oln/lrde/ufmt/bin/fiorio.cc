#include <cstdlib>

#include <ntg/int.hh>
#include <ntg/color/rgb.hh>

#include <oln/basics2d.hh>
#include <oln/level/fill.hh>

// Max-tree computation base on Fiorio's and Gustedt's algorithm.
#if defined(FIORIO_VERSION) && FIORIO_VERSION == 2
# include <oln/lrde/ufmt/fiorio-2.hh>
#else
# include <oln/lrde/ufmt/fiorio-1.hh>
#endif


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm "//"level-roots.pbm"
	    << std::endl
	    << "Max-tree computation with Fiorio's and Gustedt's algorithm."
	    << std::endl;
  // FIXME: get precise description...
  exit(1);
}


int main(int argc, char* argv[])
{
//   if (argc != 3)
//     usage(argv);
  if (argc != 2)
    usage(argv);

  typedef oln::image2d<ntg::int_u8> image_type;

  image_type input = oln::io::load(argv[1]);

  typedef oln::lrde::ufmt::fiorio<image_type> algorithm_type;

  algorithm_type run(input);
  std::cout << "npoint                         = " << run.ima().npoints()
	    << std::endl;
  run.go();
  std::cout << "n level roots (first version)  = " << run.n_level_roots1()
	    << std::endl
	    << "n level roots (second version) = " << run.n_level_roots2()
	    << std::endl;

//   // Save the image of level roots.
//   oln::save(run.level_roots(), argv[2]);

#if 0
  // Create an image of components.
  image_type output (input.size());
  oln::level::fill (output, 0);
  unsigned color = 1;
  oln_iter_type_(image_type) p(algo.parent());
  for_all(p) 
    {
      oln::point2d root = algo.parent()[p];
      // Assign a color to the root point of the component if not yet
      // done.
      if (output[root] == 0)
	output[root] = color;
      // If P is not a root, assign it the color of its root.
      if (p != root)
	output[p] = output[root];

      // Increment the color, handling overflows (sigh).  This is
      // quick and dirty, but allows us to view the output image
      // easily.
      color =
	color >= 255 ?
	1 :
	color + 1;
    }
#endif

#if 0
  algo.area_opening (5);
  oln::io::save (algo.ima(), argv[2]);
#endif
}
