// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

# include <mlc/type.hh>
# include <ntg/optraits.hh>
# include <ntg/optraits_builtins.hh>
# include <oln/convert/conversion.hh>
# include <oln/convert/force.hh>

namespace oln {
  
  using namespace ntg;

  namespace convert {

    template<class Output, class Inferior = mlc::bottom>
    struct stretch : public conversion_to_type<Output,force<Output, Inferior> >
    {
      typedef Inferior inferior;

      template< class Input >
      Output operator() (const Input& v) const {

	return Output(cast::rbound<Output, float>
		      (
		       double(v - optraits<Input>::min())
		       / double(optraits<Input>::max() - optraits<Input>::min())
		       * (optraits<Output>::max() - optraits<Output>::min())
		       + optraits<Output>::min())
		      );
      }
      
      static std::string name() {
        return std::string("stretch<") + typename_of<Output>() + ", "
          + typename_of<Inferior>() + ">";
      }
    };


  } // end of namespace convert

} // end of namespace oln


#endif // OLENA_CONVERT_STRETCH_HH
