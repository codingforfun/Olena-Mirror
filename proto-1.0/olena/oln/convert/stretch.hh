// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVERT_STRETCH_HH
# define OLENA_CONVERT_STRETCH_HH

# include <oln/core/ch_value_type.hh>

# include <oln/basics.hh>
# include <ntg/basics.hh>

# include <oln/convert/abstract/conversion.hh>

# include <set>
# include <vector>

namespace oln {

  namespace convert {

    // FIXME: Documentation (see in Olena 0.10).

    /// Functor to stretch a value from a range Input to a range Output.
    template<class Output>
    struct stretch
      : public abstract::conversion_to_type<Output, stretch<Output> >
    {
      template <class Input>
      Output
      doit(const Input& v) const
      {
	return Output(ntg::cast::rbound<Output, float>(
          double(v - ntg_min_val(Input)) /
	  double(ntg_max_val(Input) - ntg_min_val(Input)) *
	  (ntg_max_val(Output) - ntg_min_val(Output)) +
	  ntg_min_val(Output)));
      }

      static std::string
      name()
      {
	// FIXME: Exact is not an integre type !
	// return std::string("stretch<") + ntg_name(Output) + ", " +
	//   Exact::name() + ">";
	return std::string("stretch<") + ntg_name(Output) + ">";
      }
    };


    // FIXME: Documentation (see in Olena 0.10).

    /// Stretch the value of an image.
   template<class DestValue, class I>
   typename ch_value_type<I, DestValue>::ret
   stretch_balance(const oln::abstract::scalar_valued_image<I>& in,
		   const oln_type_of(I, value)& min_in =
		     ntg_min_val(oln_type_of(I, value)),
		   const oln_type_of(I, value)& max_in =
		     ntg_max_val(oln_type_of(I, value)),
		   const DestValue& min_out = ntg_min_val(DestValue),
		   const DestValue& max_out = ntg_max_val(DestValue))
    {
      ntg_is_a(DestValue, ntg::non_vectorial)::ensure();

      typename ch_value_type<I, DestValue>::ret out(in.size());

      //FIXME: I would like to remove the static_cast.
      std::vector<ntg_cumul_type(DestValue)>
	tab(static_cast<int>(max_in - min_in + 1));
      typedef typename std::set<oln_type_of(I, value)> set_type;
      set_type s;
      oln_type_of(I, piter) it(in.size());

      for_all_p (it)
	if (in[it] <= max_in && in[it] >= min_in)
	  s.insert(in[it]);
      if (s.size() == 1)
	{
	  for_all_p (it)
	    out[it] = ntg_zero_val(DestValue);
	  return out;
	}
      {
	unsigned cpt = 0;
	for (typename set_type::const_iterator it(s.begin());
	     it != s.end(); ++it, ++cpt)
	  tab[*it - min_in] = cpt * (max_out - min_out) / (s.size() - 1);
      }
      for_all_p (it)
	if (min_in <= in[it])
	  {
	    if (in[it] <= max_in)
	      out[it] = tab[in[it].value() - min_in] + min_out;
	    else
	      out[it] = max_out;
	  }
      else
	out[it] = min_out;
      return out;
    }

  } // end of namespace oln::convert.

} // end of namespace oln.

#endif // OLENA_CONVERT_STRETCH_HH
