#ifndef PROTO_OLN_CORE_ABSTRACT_DPOINT_HH
# define PROTO_OLN_CORE_ABSTRACT_DPOINT_HH

# include <oln/core/macros.hh>
# include <oln/core/abstract/dpoint_like.hh>
# include <oln/core/abstract/point_like.hh>
# include <oln/core/macros.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct dpoint : public mlc::any__best_speed<E>
    {
      coord delta() const
      {
	return this->exact().delta_impl();
      }

      bool operator==(const dpoint& rhs) const
      {
	return this->exact().op_eq_impl(rhs.exact());
      }

      bool operator!=(const dpoint& rhs) const
      {
	return ! this->operator==(rhs);
      }

      bool operator<(const dpoint& rhs) const
      {
	return this->exact().op_less_impl(rhs.exact());
      }

      const E operator-() const
      {
	return this->exact().op_minus_impl();
      }
    protected:
      dpoint() {}

    protected:
      coord	data_[oln_dim_value(E)];
    };

  } // end of namespace abstract


} // end of namespace oln


template <typename P, typename D>
inline
const P operator+(const oln::abstract::point<P>& p,
		  const oln::abstract::dpoint<D>& dp)
{
  //FIXME faut niclure les methodes avant ???
  //return oln::op_plus_impl(p.exact(), dp.exact());
}

template <typename P, typename D>
inline
const P operator+(const oln::abstract::point<P>& p,
		  const oln::abstract::dpoint_like<D>& dpl)
{
  return p + oln_dpoint_type(D)(dpl);
}

template <typename P, typename D>
inline
const oln_point_type(P) operator+(const oln::abstract::point_like<P>& pl,
				  const oln::abstract::dpoint<D>& dp)
{
  return oln_point_type(P)(pl) + dp;
}

template <typename P, typename D>
inline
const oln_point_type(P) operator+(const oln::abstract::point_like<P>& pl,
				  const oln::abstract::dpoint_like<D>& dpl)
{
  return oln_point_type(P)(pl) + oln_dpoint_type(D)(dpl);
}


#endif // ndef PROTO_OLN_CORE_ABSTRACT_DPOINT_HH
