#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/level/compare.hh>
#include <ntg/all.hh>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;

using std::cout;
using std::endl;
using std::flush;
using std::string;

template<typename E>
bool compare(const abstract::image<E>& a, 
	     const abstract::image<E>& b)
{
  return level::is_equal(a, b);
}

template<class E>
bool compare(const abstract::struct_elt<E>& a, 
	     const abstract::struct_elt<E>& b)
{
  return a == b;
}

template< typename T >
bool
loadsave(const string& name, const string& savename)
{
  bool fail = false;

  T im, im2;

  cout << T::name() << endl;
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
	  if (compare(im, im2))
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
		  if (compare(im2, im))
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
