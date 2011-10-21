#include "config.h"

extern "C"
{
#include <libgimp/gimp.h>
}

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#undef MLN_WO_GLOBAL_VARS
#include <gimp-image.hh>

#include <QApplication>
#include <QMainWindow>
#include <QGtkStyle>
#include <QObject>


#include <mln/core/image/image2d.hh>
#include <mln/debug/filename.hh>
#include <mln/math/pi.hh>
#include <mln/data/paste.hh>
#include <mln/border/resize.hh>
#include <mln/io/ppm/save.hh>

#include <preferences_dialog.hh>

extern "C"
{
  static void query_proc(void);
  static void run_proc(const gchar* name,
		       gint nparams,
		       const GimpParam* param,
		       gint* nreturn_vals,
		       GimpParam** return_vals);
}

// int copy_to_gimp(QImage& image);

GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,
  NULL,
  query_proc,
  run_proc
};


// FIXME: Useful ?
void qisMessageHandler(QtMsgType type, const char *msg )
{
  (void) type;
  (void) msg;
  //simply ignore all messages
}


int main(int argc, char **argv)
{
  int result;

  std::cout << "Main is called" << std::endl;

  // FIXME: useful?
  //install the message handler, if the the environment variable
  //DEBUG_QUITEINSANE_GIMPPLUGIN isn't set
  const char* env_v = getenv("DEBUG_QUITEINSANE_GIMPPLUGIN");
  if(env_v == NULL)
    qInstallMsgHandler(qisMessageHandler);

  QApplication app(0, 0);
  //app.setQuitOnLastWindowClosed(false);


  result = gimp_main(&PLUG_IN_INFO, argc, argv);
  std::cout << "Exiting gimp main" << std::endl;

  return result;
}



extern "C"
{
  static void query_proc(void)
  {
    static GimpParamDef args[] =
      {
	{ GIMP_PDB_INT32,    "run_mode",    "Interactive,non-interactive"},
	{ GIMP_PDB_IMAGE,    "image_id",    "Input image" },
	{ GIMP_PDB_DRAWABLE, "drawable_id", "Input drawable" }
      };

    static GimpParamDef *return_vals = NULL;
    static int nargs = sizeof(args) / sizeof(args[0]);
    static int nreturn_vals = 0;

    gimp_install_procedure("Scribo",
			   "SCRIBO Frontend",
			   "Document Image Analysis",
			   "Olena Team",
			   "Copyright EPITA/LRDE",
			   "2011",
			   "_Analyse Layout...",
			   "RGB*, GRAY*",
			   GIMP_PLUGIN,
			   nargs, nreturn_vals,
			   args, return_vals);

    gimp_plugin_menu_register ("Scribo",
			       "<Image>/Olena/Scribo/");

  }


  static void run_proc(const gchar* name,
		       gint nparams,
		       const GimpParam* param,
		       gint* nreturn_vals,
		       GimpParam** return_vals)
  {
    static GimpParam  values[1];
    GimpRunMode       run_mode;

    (void) name;
    (void) nparams;

    /* Setting mandatory output values */
    *nreturn_vals = 1;
    *return_vals  = values;

    /* Getting run_mode - we won't display a dialog if
     * we are in NONINTERACTIVE mode */
    run_mode = (GimpRunMode)param[0].data.d_int32;

    // Prepare for setting return status.
    values[0].type = GIMP_PDB_STATUS;
    values[0].data.d_status = GIMP_PDB_CALLING_ERROR;

    // ==========================
    // Begin of GIMP2Milena image
    // ==========================

      // FIXME: move into the switch/case below.
      GimpPixelRgn      in;
      gint32 x1, y1, x2, y2;

      gint32 drawable_id = param[2].data.d_drawable;
      GimpDrawable *drawable = gimp_drawable_get(drawable_id);

      gimp_drawable_mask_bounds(drawable_id, &x1, &y1, &x2, &y2);
      gint32 width = drawable->width;
      gint32 height = drawable->height;

      gimp_pixel_rgn_init(&in, drawable, x1, y1, x2, y2, FALSE, FALSE);

      typedef mln::gimp_image<GIMP_RGB_IMAGE> I;
      I tmp(&in);
      mln::image2d<mln::value::rgb8> input(height, width);
      std::cout << "Paste data into Olena image" << std::endl;
      mln::data::paste(tmp, input);
      mln::io::ppm::save(input, "/tmp/gimp_out.ppm");
      std::cout << "Paste data into Olena image: Done." << std::endl;

      // // Create a new image
      // gint32 new_image_id = gimp_image_new(width,
      // 					   height,
      // 					   gimp_image_base_type(param[1].data.d_image));
      // /* create new layer */
      // gint32 new_layer_id = gimp_layer_new(new_image_id,
      // 					   "titi",
      // 					   width, height,
      // 					   gimp_drawable_type(drawable_id),
      // 					   100,
      // 					   GIMP_NORMAL_MODE);
      // gimp_image_add_layer(new_image_id, new_layer_id, 0);
      // GimpDrawable *new_drawable = gimp_drawable_get(new_layer_id);
      // GimpPixelRgn new_region;
      // gimp_pixel_rgn_init(&new_region, new_drawable, x1, y1, x2, y2, TRUE, TRUE);

      // mln::border::resize(input, 0);

      // gimp_pixel_rgn_set_rect(&new_region,
      // 			      (const guchar *) input.buffer(),
      // 			      0,
      // 			      0,
      // 			      new_region.w,
      // 			      new_region.h);

      // /* Tidy up dirty drawable */
      // gimp_drawable_flush(new_drawable);
      // gimp_drawable_merge_shadow(new_drawable->drawable_id, TRUE);
      // gimp_drawable_update(new_drawable->drawable_id, x1, y1, x2, y2);
      // gimp_drawable_detach(new_drawable);

      // /* free buffer */
      // gimp_displays_flush();
      // gimp_display_new(new_image_id);

    // ==========================
    // end of GIMP2Milena image
    // ==========================



    switch (run_mode)
    {
      case GIMP_RUN_INTERACTIVE:
      {
	preferences_dialog dialog;
	dialog.set_current_image(input, param[1].data.d_image);
	dialog.show();

	// Run application.
	qApp->setStyle(new QGtkStyle());
	std::cout << "Running app" << std::endl;
	qApp->exec();
	std::cout << "Exiting app" << std::endl;

	values[0].data.d_status = GIMP_PDB_SUCCESS;
      }
      break;

      case GIMP_RUN_NONINTERACTIVE:
      {
      }
      break;

      case GIMP_RUN_WITH_LAST_VALS:
      {
      }
      break;

      default:
      {
      }
      break;
    }
  }
}



// int copy_to_gimp(QImage& image)
// {
//   int is_rgb = 0;
//   int x,y;
//   int remaining;
//   size_t tile_size;
//   GimpImageBaseType image_type = GIMP_GRAY;
//   GimpImageType drawable_type = GIMP_GRAY_IMAGE;
//   gint32 layer_ID;
//   gint32 image_ID;
//   GimpDrawable *drawable;
//   guchar *tile;
//   GimpPixelRgn region;
//   unsigned int tile_offset = 0;

//   tile_size = image.width() * gimp_tile_height();
//   if(image.depth() == 32)
//   {
//     is_rgb = 1;
//     tile_size *= 3;
//     image_type    = GIMP_RGB;
//     drawable_type = GIMP_RGB_IMAGE;
//   }

//   image_ID = gimp_image_new(image.width(),image.height(),image_type);
//   layer_ID = gimp_layer_new(image_ID,"Background",image.width(),image.height(),
//                             drawable_type, 100.0, GIMP_NORMAL_MODE);
//   gimp_image_set_resolution(image_ID,
// 			    int(double(image.dotsPerMeterX()) * 2.54 / 100.0),
// 			    int(double(image.dotsPerMeterY()) * 2.54 / 100.0));
//   gimp_image_add_layer(image_ID, layer_ID, 0);
//   drawable = gimp_drawable_get(layer_ID);
//   gimp_pixel_rgn_init(&region, drawable, 0, 0, drawable->width, drawable->height, TRUE, FALSE);
//   tile = g_new(guchar, tile_size);
//   x=0;
//   y=0;
//   for(y = 0; y < image.height(); y++)
//   {
//     for(x = 0; x < image.width(); x++)
//     {
//       QRgb rgb = image.pixel(x,y);
//       if(is_rgb == 1)
//       {
//         tile[tile_offset] = (guchar)qRed(rgb);
//         ++tile_offset;
//         tile[tile_offset] = (guchar)qGreen(rgb);
//         ++tile_offset;
//         tile[tile_offset] = (guchar)qBlue(rgb);
//         ++tile_offset;
//       }
//       else
//       {
//         tile[tile_offset] = (guchar)qGray(rgb);
//         ++tile_offset;
//       }
//     }
//     int tile_height = gimp_tile_height();
//     if ((y+1) % tile_height == 0)
//     {
//       gimp_pixel_rgn_set_rect(&region, tile,0,(y+1) - tile_height,image.width(),tile_height);
//       tile_offset = 0;
//     }
//   }
//   remaining = image.height() % gimp_tile_height();

//   if (remaining)
//   {
//     gimp_pixel_rgn_set_rect(&region, tile, 0, image.height() - remaining,
//                             image.width(), remaining);
//   }

//   gimp_drawable_flush(drawable);
//   gimp_display_new(image_ID);
//   gimp_drawable_detach(drawable);
//   g_free(tile);
//   tile = 0;
//   return 0;
// }
