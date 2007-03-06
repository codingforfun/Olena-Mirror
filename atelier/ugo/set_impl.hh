#ifndef   	SET_IMPPL_HH_
# define   	SET_IMPPL_HH_

#include "../local/scoop.hh"

namespace ugo
{
  namespace behavior
  {
    struct identity;
    struct mult_identity;
    struct stack;
  }

  namespace automatic
  {
    template <typename Exact>
    struct set_impl<Image, behavior::stack, Exact> : public virtual any<Exact>
    {
    };
  }
}
#endif	    /* !SET_IMPPL_HH_ */
