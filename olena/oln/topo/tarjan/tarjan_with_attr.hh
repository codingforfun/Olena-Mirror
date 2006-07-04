// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH
# define OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH

# include <oln/topo/tarjan/tarjan.hh>
# include <oln/morpho/attributes.hh>
# include <ntg/basics.hh>
# include <oln/level/fill.hh>
# include <oln/utils/histogram.hh>

# include <vector>
# include <map>

namespace oln {

  namespace topo {
    /// \brief Implementation of tarjan set.
    namespace tarjan {

      /// Abstract classes for tarjan based algorithms.
      namespace abstract {

	/*!
	** \brief Abstract  class to perform a tarjan  algorithm on an
	** image with attribute computing .
	**
	** \param Exact Exact type of the class.
	*/
	template<class Exact>
	struct tarjan_with_attr: public tarjan<mlc_exact_vt_type(tarjan_with_attr<Exact>, Exact) >
	{
	  typedef oln_tarjan_input_type(Exact)		input_type; ///< Type of input image.
	  typedef oln_point_type(input_type)		point_type; ///< Point type of image to process.
	  typedef oln_value_type(input_type)		data_type; ///< Data type of the input image.

	  typedef oln_concrete_type(input_type)		image_type; ///< Concrete type of the input image.
	  typedef oln_tarjan_output_type(Exact)		image_out_type; ///< Type of output image.
	  typedef oln_tarjan_attr_type(Exact)		attr_type; ///< Type of attribute to use.
	  typedef attr_env_type(attr_type)		env_type; ///< Environment associated to attribute type.
	  typedef oln_value_type(image_out_type)	comp_type; ///< Type of components.

	  typedef mlc_exact_vt_type(tarjan_with_attr<Exact>,
				    Exact)		exact_type; ///< Exact type of the class.
	  typedef tarjan<exact_type>			super_type; ///< Type of parent class.

	  // Make parent class able to call implementations.
	  friend class tarjan<exact_type>;
	  //friend class super_type;

	  /*!
	  ** \brief Return the attribute value associated to a
	  ** component \a i.
	  */
	  const attr_type &get_attr(const comp_type &i) const
	    {
	      return data_[find_root(i)];
	    }

	protected:

	  /*! \brief Constructor.
	  **
	  ** Initialization of data structures.
	  **
	  ** \arg ima Image to process.
	  ** \arg env Environment to use.
	  **
	  ** \warning  This  constructor  is protected,  because  this
	  ** class is abstract.
	  */
	  tarjan_with_attr(const image_type &ima,
			   const env_type &env):
	    capacity_chunk((ima.npoints() < 100) ? ima.npoints() : (ima.npoints() / 100)),
	    capacity(capacity_chunk),
	    input_(ima),
	    parent_(),
	    to_comp_(ima.size()),
	    comp_value_(),
	    env_(env)
	  {
	    parent_.reserve(capacity);
	    comp_value_.reserve(capacity);
	    data_.reserve(capacity);
	  }

	  /*!
	  ** \brief Implementation of is_proc().
	  **
	  ** \arg p Point you want to check.
	  **
	  ** \warning Do not call this method, use is_proc() instead.
	  */
	  bool is_proc_impl(const point_type &p) const
	  {
	    return to_comp_[p] != ntg_sup_val(comp_type);
	  };

	  /*!
	  ** \brief Implementation of find_root().
	  **
	  ** \arg x The component you want to find the root.
	  **
	  ** \warning  Do  not   call  this  method,  use  find_root()
	  ** instead.
	  */
	  comp_type
	  find_root(const comp_type& x) const
	  {
	    if (parent_[x] != x)
	      return parent_[x] = find_root(parent_[x]);
	    return x;
	  }

	  /*!
	  ** \brief Implementation of get_compute().
	  **
	  ** \arg Ng Neighborhood to use.
	  **
	  ** \warning  Do  not  call  this method,  use  get_compute()
	  ** instead.
	  */
	  template<class N>
	  image_out_type
	  get_compute_impl(const oln::abstract::neighborhood<N> &Ng)
	  {
	    std::vector<point_type>	I(this->get_processing_order());

	    level::fill(to_comp_, ntg_sup_val(comp_type));
	    to_comp_.border_adapt_assign(Ng.delta(), ntg_sup_val(comp_type));
	    ncomps_ = 0;
	    parent_.push_back(0);
	    comp_value_.push_back(0);
	    data_.push_back(attr_type());

	    // We are ready to perform stuff
	    for (unsigned int p = 0; p < I.size(); ++p)
	      {
		point_type p_p = I[p];
		mark_set(p_p);

		oln_neighb_type(N) Q_prime(Ng, p_p);
		for_all (Q_prime)
		  if (is_proc(Q_prime))
		    this->uni(Q_prime.cur(), p_p);
		if (to_comp_[p_p] == (ncomps_ + 1)) // new component
		  ++ncomps_;
		else
		  {
		    comp_value_.pop_back();
		    parent_.pop_back();
		    data_.pop_back();
		  }
	      }

	    // Resolving phase
	    std::map<comp_type, comp_type>		cmps;
	    ncomps_ = 0;
	    for (int p = I.size() - 1; p >= 0; --p)
	      {
		point_type p_p = I[p];
		if (cmps.find(find_root(to_comp_[p_p])) == cmps.end())
		  {
		    cmps[comp_value_[find_root(to_comp_[p_p])]] = ncomps_;
		    comp_value_[find_root(to_comp_[p_p])] = ncomps_++;
		  }
	      }

	    image_out_type output(input_.size());
	    for (int p = I.size() - 1; p >= 0; --p)
	      {
		point_type p_p = I[p];
		output[p_p] = comp_value_[find_root(to_comp_[p_p])];
	      }
	    return output;
	  }

	  /*!
	  ** \brief Implementation of ncomps_impl().
	  **
	  ** \warning Do not call this method, use ncomps() instead.
	  */
	  comp_type ncomps_impl() const
	  {
	    return ncomps_;
	  }

	  unsigned					capacity_chunk;
	  unsigned					capacity;
	  const image_type				&input_;
	  mutable std::vector<comp_type>			parent_;
	  typename oln::mute<input_type, comp_type>::ret		to_comp_;// comp number from a point
	  comp_type					ncomps_;
	  std::vector<oln_value_type(image_out_type)>	comp_value_;
	  std::vector<attr_type>			data_;
	  env_type					env_;
	};
      } // !abstract

    } // end of namespace tarjan

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH
