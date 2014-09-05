#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;	
using namespace mln;
using value::rgb8;    

cv::Mat milena_to_opencv(image2d<value::rgb8> mln_img) {

    cv::Mat cv_img(mln_img.nrows(), mln_img.ncols(), CV_8UC3);
    
    for (unsigned x = 0; x < mln_img.ncols(); ++x) {
        for (unsigned y = 0; y < mln_img.nrows(); ++y) {
            Vec3b intensity(mln_img(mln::point2d(y, x)).blue(),
		            mln_img(mln::point2d(y, x)).green(),
		            mln_img(mln::point2d(y, x)).red());
            cv_img.at<Vec3b>(y, x) = intensity;
        }
    }
    return cv_img;
}
