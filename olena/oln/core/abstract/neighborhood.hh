#ifndef PROTO_OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH
# define PROTO_OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH


namespace oln {

  namespace abstract {

    template <typename E>
    struct neighborhood : public struct_elt<E>
    {
      E& add(const oln_dpoint_type(E)& dp)
      {
	assert(dp != -dp);
	this->sdps_.insert(dp);
	this->sdps_.insert(-dp);
	this->change_ = true;
	return this->exact();
      }

      const oln_window_type(E) window() const
      {
	oln_window_type(E) win;
	win.clone_(*this);
	return win;
      }

    protected:
      neighborhood() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH
