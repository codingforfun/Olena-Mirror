#ifndef PROTO_OLN_CORE_ABSTRACT_PITER_HH
# define PROTO_OLN_CORE_ABSTRACT_PITER_HH

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>


namespace oln {


  template <>
  struct default_props < cat::piter >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type size_type;
  };


  namespace abstract {

    template <typename E>
    struct piter : public mlc::any__best_memory<E>
    {
      
      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	precondition(this->is_ok());	
	this->exact().impl_next();
      }

      bool is_ok() const
      {
	return this->exact().impl_is_ok();
      }

      operator point_type() const
      {
	precondition(this->is_ok());
	return this->p_;
      }

      void invalidate()
      {
	this->exact().impl_invalidate();
	postcondition(not this->is_ok());
      }

    protected:

      typedef piter<E> self_type;
      typedef oln_point_type(self_type) point_type;
      typedef oln_size_type(self_type) size_type;

      piter(const size_type& s) :
	s_(s),
	p_()
      {
	this->invalidate();
      }

      const size_type s_;
      point_type p_;

    };
  }
}


#endif // ndef PROTO_OLN_CORE_ABSTRACT_PITER_HH
