#include <oln/basics2d.hh>
#include <oln/level/compare.hh>
#include <oln/value/nrgb.hh>
#if HAVE_ZLIB
# include <oln/io/gz.hh>
#endif
#include "check.hh"

using namespace oln;
using namespace std;

template< typename T >
static bool
loadsave(const string& name, const string& savename)
{
  bool fail = false;

  T im, im2;

  cout << internal::debug_type< T >::name() << endl;
  cout << "  load(" << name << ")" << flush;
  if (! io::load(im, name))
    {
      cout << "FAILED";
      fail = true;
    }
  else
    {
      cout << "OK" << endl << "  reload " << flush;
      if (! io::load(im2, name))
	{
	  cout << "FAILED";
	  fail = true;
	}
      else
	{
	  cout << "OK" << endl << "  integrity " << flush;
	  if (level::is_equal(im, im2))
	    cout << "OK";
	  else
	    {
	      cout << "FAILED";
	      fail = true;
	    }
	  cout << endl <<"  save(" << savename << ") " << flush;
	  if (! io::save(im, savename))
	    {
	      cout << "FAILED";
	      fail = true;
	    }
	  else
	    {
	      cout << "OK" << endl << "  reload " << flush;
	      if (! io::load(im2, savename))
		{
		  cout << "FAILED";
		  fail = true;
		}
	      else
		{
		  cout << "OK" << endl << "  compare " << flush;
		  if (level::is_equal(im2, im))
		    cout << "OK";
		  else
		    {
		      cout << "FAILED";
		      fail = true;
		    }
		}
	    }
	}
    }
  cout << endl;
  return fail;
}
