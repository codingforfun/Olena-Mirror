#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}
 
image2d<value::rgb8> copy_AvFrame_to_image2d_rgb(AVFrame *ffmpeg_img) {
    
    image2d<value::rgb8> mln_img(ffmpeg_img->height, ffmpeg_img->width);
    
    value::rgb8 *img_mln_ptr = &mln_img(mln_img.domain().pmin());
    unsigned img_mln_nextline = mln_img.delta_offset(dpoint2d(+1, 0));
    unsigned nrows = geom::nrows(mln_img);
    uint8_t *img_ffmpeg_ptr = ffmpeg_img->data[0];

    for (unsigned int row = 0; row < nrows; ++row) {
        std::memcpy(img_mln_ptr, img_ffmpeg_ptr, ffmpeg_img->width * 3);
        img_mln_ptr += img_mln_nextline;
        img_ffmpeg_ptr += ffmpeg_img->linesize[0];
    }
    return mln_img;
}
