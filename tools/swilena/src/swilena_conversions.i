%include swilena_image.i

%{
#include <oln/convert/basics.hh>  
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/utils/copy.hh>
#include <iostream>
%}
%inline %{
namespace swilena
{
  enum conversion_type {
    STRETCH,
    FORCE,
    BOUND
  };

  template<typename To, typename From>
  To convert(To& to, const From& i, conversion_type f = FORCE)
    {
      switch(f)
	{
	case STRETCH:
	  std::cerr << "[unimplemented]" << std::endl;
	  //       to = oln::apply(oln::convert::stretch<Value(To) >(), i);
	  break;
	case FORCE:
	  to = oln::apply(oln::convert::force<Value(To) >(), i);
	  break ;
	case BOUND:
	  std::cerr << "[unimplemented]" << std::endl;
	  //      to = oln::apply(oln::convert::bound<Value(To) >(), i);
	  break ;
	  
	default:
	  break ;
	}
      return to;
    }
}
%}

%define SimpleConvert(To, From)
     %template (convert) swilena::convert<To, From >;
%enddef

