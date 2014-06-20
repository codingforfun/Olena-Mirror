// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UPSCALING_UPSCALE_BICUBIC_2X_HH
#define MLN_UPSCALING_UPSCALE_BICUBIC_2X_HH

#include <mln/core/image/image2d.hh>
#include <cmath>


/// \file
///
/// \brief upscale image x2 with bicubic interpolation.

namespace mln {

    namespace upscaling {

        /**
         \tparam image_in_ the input image (not modified by the algorithm).

         \return An upscaled x2 image.

         \author J. Fabrizio.
         */
        template<typename I>
        mln_concrete(I)
        upscale_bicubic_x2_fastest(const Image<I>& in_image_);

#ifndef MLN_INCLUDE_ONLY

        namespace impl {

            template<typename V>
            inline V tl_bicubic_interpol(const V& v0, const V& v1, const V& v2, const V& v3) {
                double P = (v3 - v2) - (v0 - v1);
                double Q = (v0 - v1) - P;
                double R = v2 - v0;
                double S = v1;
                double result = round(P * 0.125 + Q * 0.25 + R * 0.5 + S);
                result = (result < 0) ? 0 : result;
                result = (result > mln_max(V)) ? (double) mln_max(V) : result;
                return (V) result;
            }

            template<typename I>
            mln_concrete(I)
            upscale_bicubic_x2_fastest(const Image<I>& in_image_) {

                mln_trace("upscaling::upscaling_bicubic_x2");
                const I& in_image = exact(in_image_);
                mln_precondition(in_image.is_valid());
                mln_concrete(I) output(in_image.nrows()*2,in_image.ncols()*2);
                
                data::fill(output, 0);

                mln_concrete(I) tmp;
                initialize(tmp, in_image);
                data::fill(tmp, 0);

                for (def::coord row = geom::min_row(in_image); row <= geom::max_row(in_image) ; ++row)
                    for (def::coord col = geom::min_col(in_image)+1; col < geom::max_col(in_image)-1; ++col) {
                        opt::at(tmp, row, col) = tl_bicubic_interpol(in_image(point2d(row, col - 1)), in_image(point2d(row, col)), in_image(point2d(row, col + 1)), in_image(point2d(row, col + 2)));
                    }

                for (def::coord row = geom::min_row(in_image)+1, k = 1; row < geom::max_row(in_image)-1; row++, k += 2)
                    for (def::coord col = geom::min_col(in_image)+1, l = 1; col < geom::max_col(in_image); col++, l += 2) {
                        opt::at(output, k, l) = in_image(point2d(row, col));
                        opt::at(output, k , l +1 ) = tmp(point2d(row, col));
                        opt::at(output, k +1 , l ) = tl_bicubic_interpol(in_image(point2d(row - 1, col)), in_image(point2d(row, col)), in_image(point2d(row + 1, col)), in_image(point2d(row + 2, col)));
                        opt::at(output, k + 1, l + 1) = tl_bicubic_interpol(tmp(point2d(row - 1, col)), tmp(point2d(row, col)), tmp(point2d(row + 1, col)), tmp(point2d(row + 2, col)));
                    }

		//                io::magick::save(output,"upscale.tiff");
                
                
                return output;
            }

        }// end of namespace mln::upscaling::impl


        namespace internal {

            template<typename I>
            mln_concrete(I)
            upscale_bicubic_dispatch(const Image<I>& in_image_
                    , mln::trait::image::speed::any) {
                mlc_abort(I)::check();
            }

            template<typename I>
            mln_concrete(I)
            upscale_bicubic_dispatch(const Image<I>& in_image_,
                    mln::trait::image::speed::fastest) {
                return mln::upscaling::impl::upscale_bicubic_x2_fastest(in_image_);
            }

            template<typename I>
            mln_concrete(I)
            upscale_bicubic_dispatch(const Image<I>& in_image_) {
                return upscale_bicubic_dispatch(in_image_,
                        mln_trait_image_speed(I)());
            }

        }//end of namespace namespace mln::binarization::internal

        template<typename I>
        mln_concrete(I)
        upscale_bicubic_x2_fastest(const Image<I>& in_image_) {
            const I& ima = exact(in_image_);
            return mln::upscaling::internal::upscale_bicubic_dispatch(ima);
        }



#endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::upscaling


} // end of namespace mln

#endif // ! MLN_UPSCALING_UPSCALE_BICUBIC_2X_HH
