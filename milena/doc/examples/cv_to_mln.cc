#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;	
using namespace mln;
using value::rgb8;    
 
image2d<value::rgb8> opencv_to_milena(cv::Mat cv_img) {

    image2d<value::rgb8> mln_img(cv_img.rows, cv_img.cols);
    
    for (unsigned x = 0; x < mln_img.ncols(); ++x) {
        for (unsigned y = 0; y < mln_img.nrows(); ++y) {
            mln_img(mln::point2d(y, x)).red() = cv_img.at<Vec3b>(y, x).val[2];
            mln_img(mln::point2d(y, x)).green() = cv_img.at<Vec3b>(y, x).val[1];
            mln_img(mln::point2d(y, x)).blue() = cv_img.at<Vec3b>(y, x).val[0];
        }
    }
    return mln_img;
}
