#ifndef PROTO_OLN_CORE_ABSTRACT_DITER_HH
# define PROTO_OLN_CORE_ABSTRACT_DITER_HH



namespace oln {

  namespace abstract {

    template <typename E>
    struct diter : public abstract::dpoint_like<E>
    {
      const oln_dpoint_type(E) op_dpoint_impl() const
      {
	return dps_[i_];
      }

      void begin()
      {
	this->exact().begin_impl();
      }

      bool is_at_end() const
      {
	return this->exact().is_at_end_impl();
      }

      void next()
      {
	return this->exact().next_impl();
      }

    protected:

      diter() {}

      template <typename D>
      diter(const struct_elt<D>& dps) :
	dps_(dps.vector())
      {
      }

      std::vector<oln_dpoint_type(E)> dps_; // FIXME: const &
      unsigned i_;
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_DITER_HH
