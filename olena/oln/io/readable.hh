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

#ifndef OLENA_IO_READABLE_HH
# define OLENA_IO_READABLE_HH

# include <oln/config/system.hh>

# include <string>
# include <cstdlib>

namespace oln {

  namespace io {

    namespace internal {

      template<typename T>
      bool
      read_any(T& output, const std::string& name);  //forward declaration

      /*!
      ** \brief Anything.
      **
      ** This class is called by oln::load and just keep the filename to load.
      ** As soon as you will use the operator = on it, assign will be called
      ** and it will read the file.
      ** If you would like some examples to know how to use that, go to
      ** oln::abstract::iter
      */
      class anything
      {
      public:
	/*!
	** \brief Constructor
	** \todo FIXME: these constructors are required by swig
	*/
	anything() : str_() {}

	/*!
	** \brief Constructor
	*/
	anything(const anything& rhs) : str_(rhs.str_) {}

	/*!
	** \brief Constructor
	*/
	anything(const std::string& str) : str_(str) {}

	/*!
	** \brief Constructor
	*/
	anything(const char* c) : str_(c) {}

	/*!
	** \brief This function will be called when applied to an operator =
	** and load the file (str_ is the filename).
	*/
	template< typename T >
	T&
	assign(T& output) const
	{
	  read_any(output, str_);
	  ///< \todo FIXME: call output.clear()?
	  return output;
	}

      private:
	std::string str_; ///< The filename to load.
      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_READABLE_HH
