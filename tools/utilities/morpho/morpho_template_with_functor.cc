#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/laplacian.hh>
#include <oln/morpho/top_hat.hh>
#include <iostream>

#include "getopt.hh"

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef WINDOW_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

#ifndef FUNCTOR
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, 0, 'h' },
    { "win", 1, 0, 'w' },
    { 0, 0, 0, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-w filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-w filename, --win filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "filename_in                   : source file" << std::endl;
  std::cerr << "filename_out                  : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -w my_win.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  WINDOW_TYPE win = win_c8p();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hw:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try `" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'w':
        win = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(FUNCTOR, img_in, win), argv[optind + 1]);
  return 0;
}

