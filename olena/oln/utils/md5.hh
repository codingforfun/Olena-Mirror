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

#ifndef OLENA_OLN_UTILS_MD5_HH
# define OLENA_OLN_UTILS_MD5_HH

# include <ntg/all.hh>
# include <vector>

# include <oln/core/abstract/image_with_type_with_dim.hh>

# include <oln/utils/key.hh>
# include <oln/utils/buffer.hh>

namespace oln {
  namespace utils {
    /*!
    ** \brief Class used to compute a MD5 digest.
    */
    class MD5
    {
    public:
      typedef ntg::int_u32			value_type; ///< Value to work on.
      typedef ntg_storage_type_(value_type)	storage_type; ///< Representation type of value type.

      /*!
      ** \brief Constructor.
      **
      ** Initialization from a buffer.
      **
      ** \arg The buffer to use.
      */
      explicit MD5(buffer b);

      /*!
      ** \brief Compute the digest.
      */
      key execute();

      /*!
      ** \brief Generate the key.
      **
      ** \pre To avoid meaningless result, you should call execute before.
      */
      key gen_key();

    protected:
      /*!
      ** \brief initialization of a MD5 algorithm step.
      */
      void sav();

      /*!
      ** \brief Round 1 of the algorithm.
      */
      void round1();

      /*!
      ** \brief Round 2 of the algorithm.
      */
      void round2();

      /*!
      ** \brief Round 3 of the algorithm.
      */
      void round3();

      /*!
      ** \brief Round 4 of the algorithm.
      */
      void round4();

      /*!
      ** \brief A step of the algorithm.
      */
      void step();

      /*!
      ** \brief Sub step to update a register value.
      */
      template <class Fun>
      void sub_step(const Fun &fun,
		    storage_type &a,
		    const storage_type &b,
		    const storage_type &c,
		    const storage_type &d,
		    unsigned k,
		    unsigned s,
		    unsigned i);

      /*!
      ** \brief Generate a sub part of the key.
      **
      ** \arg v Vector of values (output).
      ** \arg x Data to put in v.
      ** \arg base Where to put x in v.
      */
      void gen_sub_key(std::vector<key::value_type> &v,
		       storage_type x,
		       unsigned base);


      buffer			buffer_; ///< The buffer to process.
      std::vector<storage_type>	X; ///< Vector of 16 words.
      std::vector<storage_type>	T; ///< Vector of 64 words.
      unsigned			current_; ///< Current position in the buffer.
      storage_type			aa; ///< Save of a.
      storage_type			bb; ///< Save of b.
      storage_type			cc; ///< Save of c.
      storage_type			dd; ///< Save of d.
      storage_type			a; ///< A register.
      storage_type			b; ///< B register.
      storage_type			c; ///< C register.
      storage_type			d; ///< D register.

      /*!
      ** \brief Functor for bit operations.
      **
      ** F(X,Y,Z) = XY v not(X) Z
      */
      struct F_
      {
	storage_type operator()(const storage_type &x,
				const storage_type &y,
				const storage_type &z) const
	{
	  //      return z ^ (x & (y ^ z));
	  return x & y | ~x & z;
	}
      };

      F_ F;

      /*!
      ** \brief Functor for bit operations.
      **
      ** G(X,Y,Z) = XZ v Y not(Z)
      */
      struct G_
      {
	storage_type operator()(const storage_type &x,
				const storage_type &y,
				const storage_type &z) const
	{
	  return x & z | y & ~z;
	}
      };

      G_ G;

      /*!
      ** \brief Functor for bit operations.
      **
      ** H(X,Y,Z) = X xor Y xor Z
      */
      struct H_
      {
	storage_type operator()(const storage_type &x,
				const storage_type &y,
				const storage_type &z) const
	{
	  return x ^ y ^ z;
	}
      };

      H_ H;

      /*!
      ** \brief Functor for bit operations.
      **
      ** I(X,Y,Z) = Y xor (X v not(Z))
      */
      struct I_
      {
	storage_type operator()(const storage_type &x,
				const storage_type &y,
				const storage_type &z) const
	{
	  return y ^ (x | ~z);
	}
      };

      I_ I;

    };

#include <oln/utils/md5.hxx>

    /*!
    ** \brief Compute The Md5 value of an image.
    **
    ** \param I Exact type of the image.
    **
    ** \arg im Image to process.
    **
    ** Non vectorial image version.
    */
    template <class I>
    key md5(const oln::abstract::non_vectorial_image<I> &im);

    /*!
    ** \brief Compute The Md5 value of an image.
    **
    ** \param I Exact type of the image.
    **
    ** \arg im Image to process.
    **
    ** Vectorial image version.
    */
    template <class I>
    key md5(const oln::abstract::vectorial_image<I> &im);

  } // !utils
} // !oln
#endif // !OLENA_OLN_UTILS_MD5_HH
