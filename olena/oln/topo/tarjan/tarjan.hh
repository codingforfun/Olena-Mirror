// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_TOPO_TARJAN_TARJAN_HH
# define OLENA_TOPO_TARJAN_TARJAN_HH
# include <oln/basics.hh>

// Macros for extract information on tarjan algorithms.
# define oln_tarjan_input_type(T) typename oln::topo::tarjan::tarjan_traits<T>::input_type
# define oln_tarjan_output_type(T) typename oln::topo::tarjan::tarjan_traits<T>::output_type
# define oln_tarjan_attr_type(T) typename oln::topo::tarjan::tarjan_traits<T>::attr_type


// # define oln_tarjan_input_type(T) typename oln::topo::tarjan::tarjan_traits<mlc_exact_type(T)>::input_type
// # define oln_tarjan_output_type(T) typename oln::topo::tarjan::tarjan_traits<mlc_exact_type(T)>::output_type
// # define oln_tarjan_attr_type(T) typename oln::topo::tarjan::tarjan_traits<mlc_exact_type(T)>::attr_type


namespace oln {

  namespace topo {
    /// \brief Implementation of tarjan set.
    namespace tarjan {
      /// Forward declaration for tarjan traits.
      template <typename Exact>
      struct tarjan_traits;

      /// Abstract classes for tarjan based algorithms.
      namespace abstract {
	/*!
	** \brief Top of tarjan hierarchy.
	**
	** \param I Type of image to process.
	** \param D Type of data of the wanted image.
	** \param Exact Exact type of the class.
	*/
	template <typename Exact>
	struct tarjan: public mlc_hierarchy::any<Exact>
	{
	  typedef oln_tarjan_input_type(Exact)		input_type;
	  typedef oln_tarjan_output_type(Exact)		image_out_type;
	  typedef oln_value_type(image_out_type)	comp_type;
	  typedef oln_point_type(input_type)		point_type;

	  /*!
	  ** \brief Run the algorithm.
	  **
	  **
	  ** \warning Implement get_compute_impl to be able to use
	  ** this method.
	  */
	  template<class N>
	  image_out_type
	  get_compute(const oln::abstract::neighborhood<N> &Ng)
	  {
	    mlc_dispatch(get_compute)(Ng);
	  }

	  /*!
	  ** \brief Give the number of component found.
	  **
	  ** \warning Implement ncomps_impl to be able to use
	  ** this method.
	  */
	  comp_type ncomps() const
	  {
	    mlc_dispatch(ncomps)();
	  }

	protected:
	  /*!
	  ** \brief Abstract method to get the processing order.
	  **
	  ** \warning Implement get_processing_order_impl to be able
	  ** to use this method.
	  */
	  std::vector<point_type>
	  get_processing_order()
	  {
	    mlc_dispatch(get_processing_order)();
	  }

	  /*!
	  ** \brief Mark a point as a new component.
	  **
	  ** \warning Implement mark_set_impl to be able to use this
	  ** method.
	  */
	  void
	  mark_set(const point_type &x)
	  {
	    mlc_dispatch(mark_set)(x);
	  }

	  /*!
	  ** \brief Perform an union between two components.
	  **
	  ** \warning Implement uni_impl to be able to use this
	  ** method.
	  */
	  void
	  uni(const point_type& n, const point_type& p)
	  {
	    mlc_dispatch(uni)(n, p);
	  }

	  /*!
	  ** \brief tell if a point has already been processed.
	  **
	  **
	  ** \warning Implement is_proc_impl to be able to use this
	  ** method.
	  */
	  bool is_proc(const point_type &p) const
	  {
	    mlc_dispatch(is_proc)(p);
	  };

	  /*!
	  ** \brief Make the class abstract.
	  */
	  tarjan() {}
	};
      } // end of namespace abstract
    } // end of namespace tarjan
  } // end of namespace topo
} // end of namespace oln

#endif // ! OLENA_TOPO_TARJAN_TARJAN_HH
