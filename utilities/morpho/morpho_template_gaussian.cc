#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/convol/fast_gaussian.hh>
#include <iostream>
#include <getopt.h>
#include <stdlib.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error "IMAGE_TYPE not defined"
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
    { "threshold", 1, NULL, 't' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-t threshold]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-t value, --threshold value : threshold value"
            << std::endl
            << "                              if not specified use 0.5"
            << std::endl
            << "                              by default" << std::endl;
  std::cerr << "filename_in                 : source file" << std::endl;
  std::cerr << "filename_out                : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -t 0.7"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  dfloat thres(0.5);

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "ht:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try `" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 't':
        thres = dfloat(strtod(optarg, NULL));

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(convol::FUNC(img_in, thres), argv[optind + 1]);
  return 0;
}

