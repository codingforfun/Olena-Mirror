#ifndef   	AUTOMATON_HH_
# define   	AUTOMATON_HH_

#include	"concept.hh"

namespace automaton
{
  //--automate_deterministe----------------------

# define classname  automate_det
# define current    automate_det<T>
# define super      top<T>
# define templ      template <typename T>

  stc_Header;

  stc_End;

  template <typename T>
  struct automate_det : public super
  {
  };

# include "./local/undefs.hh"

}


#endif	    /* !AUTOMATON_HH_ */
