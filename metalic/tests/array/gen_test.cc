#include "config.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gen_test_utils/errors.hh"
#include "srcdir.hh"
#include "compile.hh"

using namespace std;

#define MIN_DIMENSION 1
#define MAX_DIMENSION 3


static const struct howto
{
  typedef int (*displayer)(ostream&, int, int*, int);

  int           ok;
  displayer     element;
  displayer     lbreak;
  displayer     pbreak;
  displayer     terminate;
  const char*   verbose;
} methods[][16] =
  {
    {
      // 1D

      { 1, one_center,	no_lbreak,		no_pbreak,	normal_end      ,
	"one_center,	no_lbreak,		no_pbreak,	normal_end"     },
      { 1, if_center,	no_lbreak,		no_pbreak,	normal_end      ,
	"if_center,	no_lbreak,		no_pbreak,	normal_end"     },
      { 1, all_centers,	no_lbreak,		no_pbreak,	normal_end      ,
	"all_centers!,	no_lbreak,		no_pbreak,	normal_end"     },
      { 1, one_center,	one_lbreak,		no_pbreak,	normal_end      ,
	"one_center,	one_lbreak!,		no_pbreak,	normal_end"     },
      { 1, one_center,	no_lbreak,		no_pbreak,	no_end		,
	"one_center,	no_lbreak,		no_pbreak,	no_end!"	},
      { 1, one_center,	no_lbreak,		no_pbreak,	end_elt         ,
	"one_center,	no_lbreak,		no_pbreak,	end_elt!"       },
      { 1, one_center,	no_lbreak,		no_pbreak,	end_center      ,
	"one_center,	no_lbreak,		no_pbreak,	end_center!"    },
      { 1, one_center,	no_lbreak,		no_pbreak,	end_starter     ,
	"one_center,	no_lbreak,		no_pbreak,	end_starter!"   },
      { 1, no_comma,	no_lbreak,		no_pbreak,	normal_end      ,
	"no_comma!,	no_lbreak,		no_pbreak,	normal_end"     },
      { 1, no_element,	no_lbreak,		no_pbreak,	normal_end      ,
	"no_element!,	no_lbreak,		no_pbreak,	normal_end"     },
      { 0, 0, 0, 0, 0, 0 }
    },
    {
      // 2D

      { 1, one_center,	one_lbreak,		no_pbreak,	normal_end      ,
	"one_center,	one_lbreak,		no_pbreak,	normal_end"     },
      { 1, if_center,	one_lbreak,		no_pbreak,	normal_end      ,
	"if_center,	one_lbreak,		no_pbreak,	normal_end"     },
      { 1, all_centers,	no_lbreak,		no_pbreak,	normal_end      ,
	"all_centers!,	no_lbreak!,		no_pbreak,	normal_end"     },
      { 1, one_center,	all_lbreak,             no_pbreak,	normal_end      ,
	"one_center,	all_lbreak!,		no_pbreak,	normal_end"     },
      { 1, one_center,	too_much_lbreak,	no_pbreak,	normal_end      ,
	"one_center,	too_much_lbreak!,	no_pbreak,	normal_end"     },
      { 1, one_center,	twice_lbreak,		no_pbreak,	normal_end      ,
	"one_center,	twice_lbreak!,		no_pbreak,	normal_end"     },
      { 1, one_center,	one_lbreak,		no_pbreak,	no_end		,
	"one_center,	one_lbreak,		no_pbreak,	no_end!"	},
      { 1, one_center,	one_lbreak,		no_pbreak,	end_elt         ,
	"one_center,	one_lbreak,		no_pbreak,	end_elt!"       },
      { 1, one_center,	one_lbreak,		no_pbreak,	end_lbrk        ,
	"one_center,	one_lbreak,		no_pbreak,	end_lbrk!"      },
      { 1, one_center,	one_lbreak,		no_pbreak,	end_center      ,
	"one_center,	one_lbreak,		no_pbreak,	end_center!"    },
      { 1, one_center,	one_lbreak,		no_pbreak,	end_starter     ,
	"one_center,	one_lbreak,		no_pbreak,	end_starter!"   },
      { 1, no_comma,	one_lbreak,		no_pbreak,	normal_end      ,
	"no_comma!,	one_lbreak,		no_pbreak,	normal_end"     },
      { 1, no_element,	one_lbreak,		no_pbreak,	normal_end      ,
	"no_element!,	one_lbreak,		no_pbreak,	normal_end"     },
      { 0, 0, 0, 0, 0, 0 }
    },
    {
      // 3D

      { 1, one_center,	one_lbreak,		one_pbreak,	normal_end      ,
	"one_center,	one_lbreak,		one_pbreak,	normal_end"     },
      { 1, if_center,	one_lbreak,		one_pbreak,	normal_end      ,
	"if_center,	one_lbreak,		one_pbreak,	normal_end"     },
      { 1, one_center,	one_lbreak,		no_pbreak,	normal_end      ,
	"one_center,	one_lbreak,		no_pbreak!,	normal_end"     },
      { 1, one_center,	one_lbreak,		all_pbreak,	normal_end      ,
	"one_center,	one_lbreak,		all_pbreak!,	normal_end"     },
      { 1, all_centers,	no_lbreak,		one_pbreak,	normal_end      ,
	"all_centers!,	no_lbreak!,		one_pbreak,	normal_end"     },
      { 1, one_center,	all_lbreak,		one_pbreak,	normal_end      ,
	"one_center,	all_lbreak!,		one_pbreak,	normal_end"     },
      { 1, one_center,	too_much_lbreak,	one_pbreak,	normal_end      ,
	"one_center,	too_much_lbreak!,	one_pbreak,	normal_end"     },
      { 1, one_center,	twice_lbreak,		one_pbreak,	normal_end      ,
	"one_center,	twice_lbreak!,		one_pbreak,	normal_end"     },
      { 1, one_center,	one_lbreak,		one_pbreak,	no_end		,
	"one_center,	one_lbreak,		one_pbreak,	no_end!"	},
      { 1, one_center,	one_lbreak,		one_pbreak,	end_elt         ,
	"one_center,	one_lbreak,		one_pbreak,	end_elt!"       },
      { 1, one_center,	one_lbreak,		one_pbreak,	end_lbrk        ,
	"one_center,	one_lbreak,		one_pbreak,	end_lbrk!"      },
      { 1, one_center,	one_lbreak,		one_pbreak,	end_center      ,
	"one_center,	one_lbreak,		one_pbreak,	end_center!"    },
      { 1, one_center,	one_lbreak,		one_pbreak,	end_starter     ,
	"one_center,	one_lbreak,		one_pbreak,	end_starter!"   },
      { 1, no_comma,	one_lbreak,		one_pbreak,	normal_end      ,
	"no_comma!,	one_lbreak,		one_pbreak,	normal_end"     },
      { 1, no_element,	one_lbreak,		one_pbreak,	normal_end      ,
	"no_element!,	one_lbreak,		one_pbreak,	normal_end"     },
      { 0, 0, 0, 0, 0, 0 }
    }
  };



static void
copyfile (const string & src, ofstream & dest)
{
  ifstream ifs (src.c_str (), ios::in);
  string buffer;

  while (!ifs.eof ())
    {
      getline (ifs, buffer);
      dest << buffer << endl;
    }
}


static void
write_prelude (ofstream & ofs, const char *filename)
{
  ofs.open (filename, ios::out);
  ofs << "// -*-C++-*-" << endl
      << "// This temporary file was generated for testing purpose only."
      << endl
      << "// It's part of Olena, the static generic image processing library."
      << endl << endl;

  copyfile (srcdir + "/gen_test_utils/prelude", ofs);
}

// #include <fstream>

static bool
write_postlude_and_test (ofstream & ofs,
			 char *filename,
			 int dimension,
			 howto h,
			 int ok,
			 ofstream & success_log,
			 ofstream & fail_log, const stringstream & decl)
{
  copyfile (srcdir + "/gen_test_utils/postlude", ofs);
  ofs.close ();

  pid_t pid;
  int fd[2];
  pipe (fd);
  bool fail = false;

  if ((pid = fork ()) != -1)
    if (!pid)
      {
	dup2 (fd[1], 2);
	close (fd[0]);
	close (fd[1]);
	// FIXME: clean path generation
	std::string cmdline = compile_cmd(filename, "",
					  Isrcdir ("../..") + ' '
					  + Isrcdir ("../check") + ' ' 
					  + "-I../../../olena -I../../.. "
					  + "-L../check -lcheck " +
					  cxxflags_strict_errors());
	int ret = system (cmdline.c_str ());
	exit (WEXITSTATUS (ret));
      }
    else
      {
	close (fd[1]);
	stringstream header;
	header << endl << "=== Compiling : [" << dimension << "D]\t"
	       << h.verbose << endl
	       << "=== Expected  :  " << (ok ? "PASS" : "FAIL") << endl << endl;

	stringstream compile_mesg;
	char buffer[32];
	{
	  int r;
	  while ((r = read (fd[0], buffer, 32)) != 0)
	    compile_mesg.write (buffer, r);
	}
	int status;
	if (waitpid (pid, &status, 0) == -1)
	  abort ();
	if ((WEXITSTATUS (status) || ok) && !(WEXITSTATUS (status) && ok))
	  {
	    success_log << header.str () << decl.str ()
			<< endl << compile_mesg.
	      str () << "Exit status = " << WEXITSTATUS (status) << endl <<
	      "==========================================================================="
			<< endl;
	    cerr << (ok ? "PASS" : "XFAIL");
	  }
	else
	  {
	    fail_log << header.str () << decl.str () << endl << compile_mesg.
	      str () << "Exit status = " << WEXITSTATUS (status) << endl <<
	      "==========================================================================="
		     << endl;
	    cerr << (ok ? "FAIL" : "XPASS");
	    fail = true;
	  }
	close (fd[0]);
	cerr << ": [" << dimension << "D]\t" << h.verbose << endl;
      }
  return fail;
}



static int
fill_array (ostream & ofs, int dimension, int *geometry, howto h)
{
  int *cur_dim = (int *) malloc (dimension * sizeof (int));
  for (int i = dimension - 1; 0 <= i; --i)
    cur_dim[i] = geometry[i];
  ++cur_dim[dimension - 1];

  bool finished = false;
  int position = 0;
  int ok = h.ok;
  while (!finished)
    {
      for (int d = dimension - 1; 0 <= d; --d)
	if (0 <= --cur_dim[d])
	  break;
	else
	  {
	    cur_dim[d] = geometry[d];
	    ofs << endl << "\t\t";
	  }

      ok &= h.element (ofs, dimension, geometry, position);
      ok &= h.lbreak (ofs, dimension, geometry, position);
      ok &= h.pbreak (ofs, dimension, geometry, position);
      ++position;
      finished = true;
      for (int i = dimension - 1; 0 <= i; --i)
	finished &= (cur_dim[i] == 0);
    }
  ok &= h.terminate (ofs, dimension, geometry, position);
  return ok;
}


static bool
write_file (string filename,
	    int dimension,
	    int *geometry,
	    howto h, ofstream & success_log, ofstream & fail_log)
{
  ofstream ofs;
  write_prelude (ofs, filename.c_str ());

  int middle = geometry[0] + 1;
  for (int i = dimension - 1; 0 < i; --i)
    middle *= (geometry[i] + 1);
  middle = middle >> 1;

  stringstream decl;
  decl << "  array" << dimension << "d< array" << dimension << "d_info< ";
  for (int i = 0; i < dimension; ++i)
    decl << (geometry[i] + 1) << (i == dimension - 1 ? "" : ", ");
  for (int i = 0; i < dimension; ++i)
    if (1 == (geometry[i] & 1))
      {
	decl << ", 0";
	break;
      }
  decl << " >, int > foo = (ints_" << dimension << "d = " << endl << "\t\t";
  int ok;
  ok = fill_array (decl, dimension, geometry, h);

  ofs << decl.str ();
  return write_postlude_and_test (ofs,
				  const_cast < char *>(filename.c_str ()),
				  dimension, h, ok, success_log, fail_log,
				  decl);
}


bool
check ()
{
  string filename = "gentest_array.cc";

  ofstream success_log ("success.log", ios::out);
  ofstream fail_log ("fail.log", ios::out);

  bool fail = false;;

  for (int dimension = MIN_DIMENSION; dimension <= MAX_DIMENSION; ++dimension)
    {
      int *current_geometry = (int *) malloc (dimension * sizeof (int));
      for (int i = dimension - 1; i >= 0; --i)
	current_geometry[i] = 2;
      current_geometry[dimension - 1] = 3;

      bool finished = false;
      while (!finished)
	{
	  for (int i = dimension - 1; i >= 0; --i)
	    if (0 <= --current_geometry[i])
	      break;
	    else
	      current_geometry[i] = 2;
	  finished = true;
	  for (int i = dimension - 1; i >= 0; --i)
	    if (current_geometry[i] > 0)
	      finished = false;

	  for (int i = 0; methods[dimension - 1][i].element != 0; ++i)
	    fail |= write_file (filename, dimension, current_geometry,
				methods[dimension - 1][i],
				success_log, fail_log);
	}
      free (current_geometry);
    }
  success_log.close ();
  fail_log.close ();
  remove (filename.c_str ());
  remove ("./a.out");
  return fail;
}
