#ifndef PROTO_OLN_CORE_ABSTRACT_WINDOW_HH
# define PROTO_OLN_CORE_ABSTRACT_WINDOW_HH

# include <oln/core/abstract/dpoints.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct window : public struct_elt<E>
    {
      E& add(const oln_dpoint_type(E)& dp)
      {
	this->sdps_.insert(dp);
	this->change_ = true;
	return this->exact();
      }

      const E operator-() const
      {
	E win;
	typename std::set<oln_dpoint_type(E)>::const_iterator i;
	for (i = this->sdps_.begin(); i != this->sdps_.end(); ++i)
	  win.window::add(-(*i)); // FIXME: !!!
	win.update();
	return win;
      }
    protected:
      window() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_WINDOW_HH
