#ifndef _GETOPT_HH
# define _GETOPT_HH

# define no_argument            0 
# define required_argument      1 
# define optional_argument      2 

extern "C" {
  struct option
  {
    const char *name;

    int has_arg;
    int *flag;
    int val;
  };
  extern char *optarg;
  extern int optind;
  extern int opterr;
  int getopt_long (int, char * const *, const char *, const struct option *, int *);
}  

#endif
