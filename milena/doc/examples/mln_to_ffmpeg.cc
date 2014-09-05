#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}

AVFrame* copy_image2d_rgb_to_AvFame(image2d<value::rgb8> mln_img) {

    int w = mln_img.ncols();
    int h = mln_img.nrows();

    AVFrame *ffmpeg_img;
    ffmpeg_img = avcodec_alloc_frame();

    ffmpeg_img->linesize[0] = w * 3;
    ffmpeg_img->width = w;
    av_image_alloc(ffmpeg_img->data, ffmpeg_img->linesize, w, h, PIX_FMT_RGB24, 1);

    uint8_t *img_ffmpeg_ptr = ffmpeg_img->data[0];

    value::rgb8 *img_mln_ptr = &mln_img(mln_img.domain().pmin());
    unsigned img_mln_nextline = mln_img.delta_offset(dpoint2d(+1, 0));
    unsigned nrows = ffmpeg_img->height = h;
    
    for (unsigned int row = 0; row < nrows; ++row) {
        std::memcpy(img_ffmpeg_ptr, img_mln_ptr, w * 3);
        img_mln_ptr += img_mln_nextline;
        img_ffmpeg_ptr += ffmpeg_img->linesize[0];
    }

    return ffmpeg_img;
}
