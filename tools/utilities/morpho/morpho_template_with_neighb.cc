#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/extrema.hh>
#include <iostream>

#include "getopt.hh"

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef NEIGHBORHOOD_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, 0, 'h' },
    { "neighb", 1, 0, 'n' },
    { 0, 0, 0, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-n filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-n filename, --neighb filename : load a neighborhood from file,"
            << std::endl
            << "                                 if not specified use neighb_c4"
            << std::endl
            << "                                 by default" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -n my_neighb.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  NEIGHBORHOOD_TYPE n = neighb_c4();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hn:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try `" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'n':
        n = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(img_in, n), argv[optind + 1]);
  return 0;
}

