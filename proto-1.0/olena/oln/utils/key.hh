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

#ifndef OLENA_OLN_UTILS_KEY_HH
# define OLENA_OLN_UTILS_KEY_HH
# include <ntg/all.hh>
# include <vector>
# include <iostream>

namespace oln {
  namespace utils {
    /*!
    ** \brief 16 bytes key
    **
    ** Object to have a nice representation of a MD5 result.
    */
    class key
    {
    public:
      typedef ntg::int_u8	value_type; ///< Used data type.

      /*!
      ** \brief Constructor used to initialize the key.
      **
      ** \pre There must at least 16 elements in data.
      ** \arg data Inuput data.
      */
      explicit key(const std::vector<value_type> &data);

      /*!
      ** \brief Constructor used to initialize the key.
      **
      ** \pre There must at least 16 elements in data.
      ** \arg data Inuput data.
      */
      explicit key(const value_type *data);

      /*!
      ** \brief [] operator.
      **
      ** \return the ith byte of the key.
      **
      ** \arg i Index of the byte wanted.
      **
      ** This is the non const version of the operator.
      */
      value_type &operator[](unsigned i);

      /*!
      ** \brief [] operator.
      **
      ** \return the ith byte of the key.
      **
      ** \arg i Index of the byte wanted.
      **
      ** This is the const version of the operator.
      */
      const value_type &operator[](unsigned i) const;

      /*!
      ** \brief Check equality between two keys.
      **
      ** \arg k Key to be compared with.
      */
      bool operator ==(const key &k);

      /*!
      ** \brief Check whether two key are different.
      **
      ** \arg k Key to be compared with.
      */
      bool operator !=(const key &k);

      /*!
      ** \brief Write of representation of the key on a stream.
      **
      ** stream Stream to put the key on.
      ** k Key to represent.
      */
      friend std::ostream &operator<<(std::ostream &stream, const key &k)
      {
	stream << "{";
	for (unsigned i = 0; i < 15; ++i)
	  stream << "0x" << std::hex << k[i] << ", ";
	stream << "0x" << std::hex << k[15] << "}";
	return stream;
      }


    protected:
      value_type	data_[16]; ///< Internal data.
    };

# include <oln/utils/key.hxx>

  } // !utils
} // !oln
#endif // !OLENA_OLN_UTILS_KEY_HH
