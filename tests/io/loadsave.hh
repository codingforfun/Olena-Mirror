#include "basics2d.hh"
#include "level/compare.hh"
#include "value/nrgb.hh"

using namespace oln;
using namespace std;

template< typename T >
static bool
loadsave(const string& name)
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
      cout << "OK" << endl << "  reload  " << flush;
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
	  cout << endl <<"  save(" << name << ")" << flush;
	  if (! io::save(im, "savetmp"))
	    {
	      cout << "FAILED";
	      fail = true;
	    }
	  else
	    {
	      cout << "OK" << endl << "  reload " << flush;
	      if (! io::load(im2, "savetmp"))
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
