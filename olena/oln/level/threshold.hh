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

#ifndef OLENA_LEVEL_THRESHOLD_HH
# define OLENA_LEVEL_THRESHOLD_HH

# include <oln/basics.hh>
# include <ntg/utils/debug.hh>

namespace oln {

  namespace level {

    /*! \brief Threshold the value of the image.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/level/threshold.hh>
    ** #include <ntg/all.hh>
    ** using namespace ntg;
    ** int main()
    ** {
    **   oln::image2d<int_u8> in = oln::load(IMG_IN "lena256.pgm");
    **   int_u8 th	= 127;
    **   rgb_8 low	= rgb_8(100, 0, 0);
    **   rgb_8 height	= rgb_8(0, 200, 255);
    **
    **   oln::image2d<rgb_8> out
    **     = apply(oln::level::threshold<int_u8, rgb_8 >(th, low, height),
    ** 	    in);
    **   save(out, IMG_OUT "oln_level_threshold.ppm");
    ** }
    ** \endcode
    ** \image html lena256_pgm.png
    ** \image latex lena256_pgm.png
    ** =>
    ** \image html oln_level_threshold.png
    ** \image latex oln_level_threshold.png
    */
    template<class Input, class Output, class Exact = mlc::final>
    class threshold : public std::unary_function<const Input&, Output>
    {
    public:
      /*! \arg Threshold any value heigher or equal to this value will
      ** return \a min.
      **  \arg min Value returned if the input is smaller than threshold.
      **  \arg max Value returned if the input is greater or equal to the
      ** threshold.
      */
      threshold(const Input& threshold,
		const Output& min = ntg_min_val(Output),
		const Output& max = ntg_max_val(Output)) :
        m_threshold(threshold), m_min(min), m_max(max)
      {}

      Output
      operator() (const Input& v) const
      {
        if (v >= m_threshold)
          return m_max;
        return m_min;
      }

      static std::string
      name()
      {
	return std::string("threshold<")
	  + ntg_name(Output) + ", "
	  + ntg_name(Input) + ", "
	  + Exact::name() + ">";
      }

    private:
      const Input m_threshold;
      const Output m_min;
      const Output m_max;
    };

  } // end of namespace convert

} // end of namespace oln

#endif // ! OLENA_LEVEL_THRESHOLD_HH
