#ifndef PROTO_OLN_CORE_ABSTRACT_STRUCT_ELT_HH
# define PROTO_OLN_CORE_ABSTRACT_STRUCT_ELT_HH

# include <set>
# include <vector>
# include <iterator>


namespace oln {

  namespace abstract {

    template <typename E>
    struct struct_elt : public mlc::any__best_memory<E>
    {
      typedef std::set<oln_dpoint_type(E)> sdps_type;
      typedef std::vector<oln_dpoint_type(E)> vdps_type;

      unsigned size() const
      {
	update();
	return vdps_.size();
      }

      unsigned card() const // FIXME: for backward compatibility
      {
	return size();
      }

      coord delta() const
      {
	update();
	return delta_;
      }

      const vdps_type vector() const
      {
	update();
	return vdps_;
      }

      // FIXME: for backward compatibility
      oln_dpoint_type(E) dp(unsigned i) const
      {
	update();
	assert(i < vdps_.size());
	return vdps_[i];
      }

      // hook:
      const sdps_type& sdps() const { return sdps_; }
      sdps_type& sdps() { return sdps_; }

      template <typename E2>
      void clone_(const struct_elt<E2>& rhs)
      {
	this->sdps_ = rhs.sdps();
	this->change_ = true;
	this->update();
      }

      void update() const
      {
	if (change_)
	  {
	    // update vdps_
	    vdps_.clear();
	    std::copy(sdps_.begin(), sdps_.end(),
		      std::back_inserter(vdps_));
	    // update delta_
	    delta_ = 0;
	    for (unsigned i = 0; i < vdps_.size(); ++i)
	      if (vdps_[i].delta() > delta_)
		delta_ = vdps_[i].delta();
	    // update change_
	    change_ = false;
	  }
      }

    protected:

      sdps_type sdps_;
      mutable vdps_type vdps_;
      mutable coord delta_;
      mutable bool change_;

      struct_elt()
      {
	delta_ = 0;
	change_ = false;
      }

    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_STRUCT_ELT_HH
