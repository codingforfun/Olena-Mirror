This file contains information about the build process of Olena.
Please read the README file for general information about Olena.

CONFIGURATION
=============

In order to prepare the build process, you need to configure the
source tree.

Assuming your Olena distribution is uncompressed in directory oln-0.7,
follow these steps:

  mkdir ../build
  cd ../build && ../oln-0.7/configure

The build process can be altered by a number of options you can pass
to the `configure' script. The following sections describe them.

     Installation path
     -----------------

     By default, Olena is installed in the standard "local" directory
     of your system. This is usually /usr/local under Unix.

     You can change this path with the following flag:
	
        --prefix=<installation prefix>

    Compiler selection and compilation flags
    ----------------------------------------

    By default, `configure' will try to use the first C++ compiler it
    encounters on your system. It will look, in order, for :

	- the value of the $CCC environment variable
	- the GNU C++ compiler (g++)
	- the `c++' or `gpp' commands on your system
	- aCC, the HP-UX standard C++ compiler
	- the `CC', `cxx', `cc++' or `cl' commands on your system
	- KAI's C++ compiler (KCC)
	- RCC, xlC_r or xlC.

    You can override the detection system by EITHER:

	- setting the environment variable $CXX, with e.g.:

	   (bash, /bin/sh:)   CXX=your-favorite-C++-compiler; export CXX
	   (csh)              setenv CXX your-favorite-C++-compiler
 
    OR

	- passing the CXX= argument to `configure', with e.g.:

	   .../configure CXX=your-favorite-C++-compiler

    For most compilers, `configure' will select debugging and minimal
    optimization (-g -O2 with g++). You can override the default C++
    flags by EITHER:

	- setting the environment variable $CXXFLAGS, with e.g.:

	   (bash, /bin/sh:)   CXXFLAGS=your-favorite-flags; export CXXFLAGS
	   (csh)              setenv CXXFLAGS your-favorite-flags
 
    OR

	- passing the CXXFLAGS= argument to `configure', with e.g.:

	   .../configure CXXFLAGS=your-favorite-flags
    
     This is especially useful for solving a number of problems, described
     in the following section.

     Using CXXFLAGS to solve compilation problems
     --------------------------------------------

     Olena needs C99:

     While Olena is written in C++, it uses a number of features (math
     functions) from ISO C99. However most C++ compilers do not enable
     these features by default.
     If your compilation fails with (e.g.) undefined references to
     "roundf", but you know what flags can activate these functions,
     add them to CXXFLAGS.

     Olena needs deep template recursion:

     The C++ design patterns used in Olena use deep template nesting
     and recursion. However, the C++ standard specifies that C++
     compiler need only handle template recursion upto 19 levels,
     which is insufficient for Olena. This is a problem for GCC 2.95
     and probably other compilers.
     Hopefully, `configure' tries to fix this automatically by adding
     "-ftemplate-depth-NN" when necessary, but other compilers than
     GCC may need other flags. If you know these flags, add them to
     CXXFLAGS.

     Debugging flags make Olena slow:

     Because Olena depends on C++ optimizations to provide the best
     performance, and enabling debugging flags often disable
     optimizations, you are advised to verride the CXXFLAGS with any
     options that gives the best optimization/conformance tradeoff.
     However, note that passing -DNDEBUG disable many sanity checks,
     while providing only a poor performance improvement.

     Speeding up the configuration process
     -------------------------------------

     `configure' can manage a cache of autodetected features and
     values. This cache speeds up `configure' runs and can be
     activated with the -C option.

     NOTE: the effects of many of the flags passed to `configure' are
     stored in the cache. If you decide to re-run `configure' with
     other flags, delete the "config.cache" file first.

     Using `configure' while developing on Olena
     -------------------------------------------

     By default, successive runs of the build process do not track
     changes to Olena headers, and thus does not rebuild files for
     which dependencies are modified.
     To enable dependency tracking, give the

              --enable-dependency-tracking 

     flag to `configure'.

     By default, the build process control files (Makefile's) are not
     automatically updated when the project control files
     (configure.ac, Makefile.am's) are modified.
     To enable automatic runs of "autoconf", "automake" and
     "autoheader" when control files need updating, use the

	      --enable-maintainer-mode

     flag with `configure'.

OPTIONAL FEATURES
=================

     Using external libraries
     ------------------------

     Several parts of Olena can make use of the Zlib compression library 
     (in Olena I/O) and the FFTW fast Fourier transforms library (in
     Olena fft transforms). Their use can be enabled with the
     following flags:

	--with-fftw
	--with-zlib

     If your version of any of these libraries is located in a
     non-standard path, you can specify it as follows:

	--with-fftw=<path-to-libfftw>
	--with-zlib=<path-to-zlib>

     Selecting Olena components
     --------------------------

     The distribution is divided into several directories, each
     pertaining to a `component'. They are all included by default,
     but can be individually excluded with appropriate flags.

     Once a directory is excluded, it is not taken in consideration in
     any stage of build, installation or release stages of "make"
     runs.

     Here is a description of the different directories:
 
     - oln/: the Olena library.

        This directory cannot (obviously) be excluded as a whole.

        It contains:

        * oln/img/: the sample images.
        * oln/oln/: the Olena headers.
        * oln/tests/: the testsuite.
 
          This latter directory can be excluded as a whole with:
                `--without-testsuite'
  
	  It contains several directories, each pertaining to a
	  category of tests. The flags `--with-tests' controls which
	  categories are enabled. The different categories are:
	
             convert,convol,io,meta,morpho,sanity,transforms,types

          They are all enabled by default (`--with-tests=all'). To
          enable only a subset of the testsuite, use (e.g.):
             `--with-tests=io,meta'

     - doc/: the documentation.

        This directory can be excluded as a whole with:
		`--without-doc'

        It contains:

	* doc/doc/: the manuals.
	  This directory can be excluded with `--without-manuals'.
        * doc/demo/: demonstration programs.
          This directory can be excluded with `--without-demo'.

     - tools/: the runtime tools.

        This directory can be excluded as a whole with:
                 `--without-tools'

         It contains:

         * tools/utilities/: command-line access to Olena functions.
           This directory can be excluded with `--without-utilities'.
         * tools/swilena/: scripting language access to Olena functions.
           This directory can be excluded with `--without-swilena'.

BUILD
=====

Once your build directory is `configure'd, you can run

    make

to recursively build all selected components.


Additionnally, you can build and run the testsuite and demonstration
programs with:

    make check

However, this process is very time- and memory- consuming. It takes up
to 20mn and 150-2O0Mb of virtual memory on a Debian GNU/Linux 1GHz
bi-P3 machine.

