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

#ifndef OLENA_CC_TARJAN_HH
# define OLENA_CC_TARJAN_HH

# include <vector>

# include <oln/basics2d.hh>
# include <oln/core/abstract/image_operator.hh>
# include <oln/level/fill.hh>

//
// Based on "Ruminations on Tarjan's Union-Find algorithm and connected
// operators" of Thierry Geraud.
//


namespace oln {

  // fwd decl
  namespace morpho {
    template <typename T, typename I, typename E> struct cc_tarjan_ret;
  }

  // FIXME: this macro is waiting for the mute.
  // (we may use a property to do it)
  #define tmp_mute(I, T) image2d< T >

  // category
  template <typename T, typename I, typename E>
  struct set_category< morpho::cc_tarjan_ret<T,I,E> >
  { typedef category::image ret; };

  // super_type
  template <typename T, typename I, typename E>
  struct set_super_type< morpho::cc_tarjan_ret<T,I,E> >
  {
    typedef abstract::image_unary_operator
              <tmp_mute(I, T), I, morpho::cc_tarjan_ret<T, I, E> >
            ret;
  };


  namespace morpho {

    template <typename T, typename I, typename E>
    struct cc_tarjan_ret
      : public abstract::image_unary_operator
                 <tmp_mute(I, T), I, cc_tarjan_ret<T, I, E> >
    {
      typedef abstract::image_unary_operator
                <tmp_mute(I, T), I, cc_tarjan_ret<T, I, E> >
              super_type;
      typedef typename super_type::output_type output_type;

      const E ng;

      cc_tarjan_ret(const abstract::image<I>& input,
	            const abstract::neighborhood<E>& ng) :
	super_type(input),
	ng(ng.exact())
	{
	}

    };


    namespace impl {


      namespace misc {

        // FIXME: This code should be generalized.
        // While iterating on image and then on a neighborhood, here we do
        // not want to see the neighborhood points ever seen.
        std::vector<dpoint2d> get_superior(const neighborhood2d& n)
        {
          std::vector<dpoint2d> output;
          for (unsigned i = 0; i < n.card(); ++i)
            {
              dpoint2d dp = n.dp(i);
              if (dp.row() < 0 || (dp.row() == 0 && dp.col() < 0))
                output.push_back(dp);
            }
          return output;
        }

        std::vector<dpoint2d> get_inferior(const neighborhood2d& n)
        {
          std::vector<dpoint2d> output;
          for (unsigned i = 0; i < n.card(); ++i)
            {
              dpoint2d dp = n.dp(i);
              if (dp.row() > 0 || (dp.row() == 0 && dp.col() > 0))
                output.push_back(dp);
            }
          return output;
        }

     } // end of misc namespace


      template <typename T, typename I, typename N>
      struct generic_cc_tarjan : public cc_tarjan_ret<T, I, N>
      {
	typedef cc_tarjan_ret<T, I, N> super_type;
	typedef typename super_type::output_type output_type;

        tmp_mute(I, oln_type_of(I, point)) parent;
        T ncomps;

	generic_cc_tarjan(const abstract::image<I>& input,
			  const abstract::neighborhood<N>& ng) :
	  super_type(input, ng),
          parent(input.size())
	{
	}

	void impl_run()
	{
	  mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();

	  output_type tmp(this->input.size()); // FIXME: trick
	  this->output = tmp;

          first_pass();
          second_pass();
	}

        void first_pass()
        {
          std::vector<dpoint2d> neighb = misc::get_inferior(this->ng);
          oln_type_of(I, bkd_piter) p(this->input.size());
          for_all_p (p)
            if (this->input[p])
            {
              make_set(p);
              for (unsigned v = 0; v < neighb.size(); ++v)
              {
                oln_type_of(I, point) n = oln_type_of(I, point)(p) + neighb[v];
                if (this->input[n])
                  do_union(n, p);
              }
            }
        }

        void second_pass()
        {
          oln_type_of(I, fwd_piter) p(this->input.size());
          level::fill(this->output, 0);
          ncomps = 0;
          for_all_p (p)
            if (this->input[p])
            {
	      oln_type_of(I, point) q = parent[p];
              // FIXME: test if ncomps > T::max()
	      this->output[p] = (q == p ? ++ncomps : this->output[q]);
            }
        }

        void make_set(const oln_type_of(I, point)& x)
        {
          parent[x] = x;
        }

        oln_type_of(I, point) find_root(const oln_type_of(I, point)& x)
        {
          if (parent[x] != x)
            {
              parent[x] = find_root(parent[x]);
              return parent[x];
            }
          return x;
        }

        void do_union(const oln_type_of(I, point)& n,
                      const oln_type_of(I, point)& p)
        {
          oln_type_of(I, point) r = find_root(n);
          if (r != p)
            parent[r] = p;
        }


      };

    } // end of namespace oln::morpho::impl


    /// Connected component labelling via Tarjan Union-Find generic routine.

    // T is the component label type (usually unsigned).
    template<typename T, typename I, typename N>
    cc_tarjan_ret<T, I, N> cc_tarjan(const abstract::image<I>& input,
			          const abstract::neighborhood<N>& ng)
    {
      impl::generic_cc_tarjan<T, I, N> tmp(input, ng);
      tmp.run();
      return tmp;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_CC_TARJAN_HH
