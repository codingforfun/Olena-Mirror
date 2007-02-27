#ifndef   	CONCEPT_HH_
# define   	CONCEPT_HH_

#include "./local/scoop.hh"

namespace automaton
{
  stc_equip_namespace;

  stc_decl_associated_type( state_t );
  stc_decl_associated_type( state_set_t );
  stc_decl_associated_type( symbole_set_t );
  stc_decl_associated_type( symbole_t );
  stc_decl_associated_type( etiquette_t );



  //--Symbole-set-------------------

  template <typename Exact>
  struct Symbole_Set : public virtual any<Exact>,
		       public automatic::get_impl<Symbole_Set, Exact>
  {
      stc_typename( symbole_t );
    protected:
      Symbole_Set() { }
  };



  //--State------------------------

  template <typename Exact>
  struct State : public virtual any<Exact>,
		 public automatic::get_impl<State, Exact>
  {
      stc_typename( etiquette_t );
    protected:
      State() {}
  };



  //--Trans_functor----------------

  template <typename Exact>
  struct Trans_functor : public virtual any<Exact>,
			 public automatic::get_impl<Trans_functor, Exact>
  {
      stc_typename( symbole_t );
      stc_typename( state_t   );

      state_t trans(state_t s, symbole_t a) { return this->exact().impl_trans ( s.exact(), a.exact() );  }
    protected:
      Trans_functor() { }
  };



  //--State_Set--------------------

  template <typename Exact>
  struct State_Set : public virtual any<Exact>,
		     public automatic::get_impl<State_Set, Exact>
  {
      stc_typename( state_t );
    protected:
      State_Set() {}
  };



  //-----------------------------------------
  //  AUTOMATON
  //-----------------------------------------

  //--Generalised Non-deterministic finite automaton

  template <typename Exact>
  //create needed attribute with types given by user
  struct GNFA : public virtual any<Exact>,
		public automatic::get_impl<GNFA, Exact>
  {
      stc_typename( state_set_t	  );
      stc_typename( symbole_set_t );
      stc_typename( state_t       );

      state_set_t	state_set;
      symbole_set_t	alpha;
      state_t		start_state;
      state_t		accept_state;

    protected:
      GNFA() {
	mlc::assert_< mlc_is_a(state_t,       State      ), ERROR_wrong_state_type       >::check();
	mlc::assert_< mlc_is_a(state_set_t,   State_Set  ), ERROR_wrong_state_set_type   >::check();
	mlc::assert_< mlc_is_a(symbole_set_t, Symbole_Set), ERROR_wrong_symbole_set_type >::check();
      }

    private:
      struct ERROR_wrong_state_set_type;
      struct ERROR_wrong_symbole_set_type;
      struct ERROR_wrong_state_type;
  };



  //--Non-deterministic Finite Automaton

  template <typename Exact>
  struct NFA : public virtual GNFA<Exact>,
	       public automatic::get_impl<NFA, Exact>
  {
      typedef GNFA<Exact> super;
    protected:
      NFA() {}
  };



  //--Deterministic Finite Automaton

  template <typename Exact>
  struct DFA : public virtual NFA<Exact>,
	       public automatic::get_impl<DFA, Exact>
  {
      typedef NFA<Exact> super;

    protected:
      DFA() {}
  };


}
#include "automaton.hh"

#endif	    /* !CONCEPT_HH_ */
