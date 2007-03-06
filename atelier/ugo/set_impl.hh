#ifndef   	SET_IMPPL_HH_
# define   	SET_IMPPL_HH_

#include "../local/scoop.hh"

namespace ugo
{
  namespace behavior
  {
    struct identity;
    struct mult_identity;
  }

  namespace automatic
  {
    template <typename Exact>
    struct set_impl<Image, behavior::identity, Exact> : public virtual any<Exact>
    {
	stc_typename( point  );
	stc_typename( value );
	stc_typename( box    );

	value impl_rvalue_access(const point& p) const{ return this->exact().image()(p) ;      }
	bool  impl_owns(const point& p) const         { return this->exact().image().owns_(p); }
	box   impl_bbox() const		              { return this->exact().image().bbox();   }
    };
    
  }
}
#endif	    /* !SET_IMPPL_HH_ */
