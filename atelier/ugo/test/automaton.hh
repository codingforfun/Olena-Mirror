#ifndef   	AUTOMATON_HH_
# define   	AUTOMATON_HH_

#include	"concept.hh"

namespace automaton
{

  //--symbole_set----------------------

# define classname  symbole_set
# define current    symbole_set<T>
# define super      top<current>
# define templ      template <typename T>

  stc_Header;

  typedef T			symbole_t;
  typedef stc::is<Symbole_Set>	category;

  stc_End;

  template <typename T>
  struct symbole_set : public super
  {
  };

# include "./local/undefs.hh"



  //--state-------------------------

# define classname  state
# define current    state<T>
# define super      top<current>
# define templ      template <typename T>

  stc_Header;

  typedef T			etiquette_t;
  typedef stc::is<State>	category;

  stc_End;

  template <typename T>
  struct state : public super
  {
  };

# include "./local/undefs.hh"



  //--state_set----------------------

# define classname  state_set
# define current    state_set<T>
# define super      top<current>
# define templ      template <typename T>

  stc_Header;

  typedef T			state_t;
  typedef stc::is<State_Set>	category;

  stc_End;

  template <typename T>
  struct state_set : public super
  {
  };

# include "./local/undefs.hh"


  //--automate_deterministe----------------------

# define classname  dfa
# define current    dfa<T>
# define super      top<current>
# define templ      template <typename T>

  stc_Header;

  typedef symbole_set<T>	symbole_set_t;
  typedef state_set<T>		state_set_t;
  typedef stc::is<DFA>		category;

  stc_End;

  template <typename T>
  struct dfa : public super
  {
  };

# include "./local/undefs.hh"

}


#endif	    /* !AUTOMATON_HH_ */
