#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/hit_or_miss.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error "IMAGE_TYPE not defined"
#endif

#ifndef WINDOW_TYPE
# error "WINDOW_TYPE not defined"
#endif

#ifndef DATA_TYPE
# error "DATA_TYPE not defined"
#endif

#ifndef FUNC
# error "FUNC not defined"
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "win1", 1, NULL, 'w' },
    { "win2", 1, NULL, 'W' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-w filename] [-W filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-w filename, --win1 filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "-W filename, --win2 filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -w my_win1.pbm -W my_win2.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  WINDOW_TYPE win1 = win_c8p();
  WINDOW_TYPE win2 = win_c8p();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hw:W:", long_options, &opt_index)) != -1)
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
        win1 = load(optarg);

      case 'W':
        win2 = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(img_in, win1, win2), argv[optind + 1]);
  return 0;
}

