// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_TAGS_HH
# define OLENA_MORPHO_TAGS_HH

# include <mlc/any.hh>
# include <mlc/cmp.hh>
# include <oln/basics.hh>

# include <oln/morpho/stat.hh> // FIXME: rename!!!



namespace oln {

  namespace morpho {


    namespace tag
    {

      // Tags for selecting a given 'morphology' due to
      // the couple of dual operations performed in erosion
      // and dilation.


      template <typename K>
      struct kind : public mlc::any<K>  // FIXME: name!
      {

	// min value in input on a window centered at p
	template <typename I, typename W>
	oln_type_of(I, value)
	  min(const abstract::image<I>& input,
	      const oln_type_of(I, point)& p,
	      const abstract::window<W>& win) const
	{
	  mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
	  return this->exact().impl_min(input, p, win);
	}

	// max value in input on a window centered at p
	template <typename I, typename W>
	oln_type_of(I, value)
	  max(const abstract::image<I>& input,
	      const oln_type_of(I, point)& p,
	      const abstract::window<W>& win) const
	{
	  mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
	  return this->exact().impl_max(input, p, win);
	}

	// min value in input of neighborhoods of p
	template <typename I>
	oln_type_of(I, value)
	  min_nbh(const abstract::image_with_nbh<I>& input,
		  const oln_type_of(I, point)& p) const
	{
	  return this->exact().impl_min_nbh(input, p);
	}

	// max value in input of neighborhoods of p
	template <typename I>
	oln_type_of(I, value)
	  max_nbh(const abstract::image_with_nbh<I>& input,
		  const oln_type_of(I, point)& p) const
	{
	  return this->exact().impl_max_nbh(input, p);
	}

      };



      struct classical_type : public kind< classical_type >
      {
	// min value in input on a window centered at p
	template <typename I, typename W>
	oln_type_of(I, value)
	  impl_min(const abstract::image<I>& input,
		   const oln_type_of(I, point)& p,
		   const abstract::window<W>& win) const
	{
	  return morpho::local_min(input, p, win);
	}

	// max value in input on a window centered at p
	template <typename I, typename W>
	oln_type_of(I, value)
	  impl_max(const abstract::image<I>& input,
		   const oln_type_of(I, point)& p,
		   const abstract::window<W>& win) const
	{
	  return morpho::local_max(input, p, win);
	}

	// min value in input of neighborhoods of p
	template <typename I>
	oln_type_of(I, value)
	  impl_min_nbh(const abstract::image_with_nbh<I>& input,
		       const oln_type_of(I, point)& p) const
	{
	  return morpho::local_min_nbh(input, p);
	}

	// max value in input of neighborhoods of p
	template <typename I>
	oln_type_of(I, value)
	  impl_max_nbh(const abstract::image_with_nbh<I>& input,
		       const oln_type_of(I, point)& p) const
	{
	  return morpho::local_max_nbh(input, p);
	}

      };

      const classical_type classical = classical_type();


      // Tags for approaches in reconstructions.

      template <typename Op> struct oper {};
      
      struct by_dilation_type : public oper< by_dilation_type > {};
      struct by_erosion_type  : public oper< by_erosion_type > {};
      

      // Tags for common canvas.
      
      template <typename A> struct algo {};
      
      struct sequential_type : public algo< sequential_type > {};
      struct hybrid_type     : public algo< hybrid_type > {};
      struct parallel_type   : public algo< parallel_type > {};
      struct unionfind_type  : public algo< unionfind_type > {};
      
    } // end of namespace oln::morpho::tag


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_EROSION_HH
