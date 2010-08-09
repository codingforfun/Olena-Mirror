// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_V2V_ACHROMATISM_HH
# define MLN_FUN_V2V_ACHROMATISM_HH

# include <mln/value/rgb8.hh>

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      struct achromatism : public Function_v2v< achromatism >
      {
	typedef float result;

	float operator()(const value::rgb8 rgb) const;
      };

# ifndef MLN_INCLUDE_ONLY

      float achromatism::operator()(const value::rgb8 rgb) const
      {
	return (math::abs(rgb.red() - rgb.green())
		+ math::abs(rgb.red() - rgb.blue())
		+ math::abs(rgb.green() - rgb.blue()))/3.0;
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_ACHROMATISM_HH