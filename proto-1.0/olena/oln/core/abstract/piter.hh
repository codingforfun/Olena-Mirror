#ifndef PROTO_OLN_CORE_ABSTRACT_PITER_HH
# define PROTO_OLN_CORE_ABSTRACT_PITER_HH

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>
# include <oln/core/macros.hh>



# define for_all(P) for(p.start(); p.is_valid(); p.next())


namespace oln {

  namespace abstract {

    template <typename E> struct piter;

  }


  template <typename E>
  struct category_type<abstract::piter<E> > { typedef cat::piter ret; };


  template <>
  struct default_props < cat::piter >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type size_type;
  };


  namespace abstract {

    template <typename E>
    struct piter : public mlc::any__best_speed<E>
    {
      typedef piter<E> self_type;
      typedef oln_point_type(E) point_type;
      typedef oln_size_type(E) size_type;

      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	precondition(this->is_valid());
	this->exact().impl_next();
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->p_;
      }

      void invalidate()
      {
	this->exact().impl_invalidate();
	postcondition(not this->is_valid());
      }

    protected:

      piter(const size_type& s) :
	s_(s),
	p_()
      {
      }

      const size_type s_;
      point_type p_;

    };
  }
}


#endif // ndef PROTO_OLN_CORE_ABSTRACT_PITER_HH
