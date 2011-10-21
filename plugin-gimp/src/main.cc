/* GIMP Plug-in Template
 * Copyright (C) 2000  Michael Natterer <mitch@gimp.org> (the "Author").
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Author of the
 * Software shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the Author.
 */

#include "config.h"

#include <string.h>
#include <stdio.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "interface.hh"

#include "plugin-intl.hh"

#include "build-image.hh"

/*  Constants  */

#define PROCEDURE_NAME   "milena"

/*  Local function prototypes  */

static void   query (void);
static void   run   (const gchar      *name,
		     gint              nparams,
		     const GimpParam  *param,
		     gint             *nreturn_vals,
		     GimpParam       **return_vals);


GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

int main(int argc, char **argv)
{
  int result;
  getchar();
  result = gimp_main(&PLUG_IN_INFO, argc, argv);
  return result;
}


static void
query (void)
{
  static GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    (gchar *)"run_mode",   (gchar *)"Interactive, non-interactive"    },
    { GIMP_PDB_IMAGE,    (gchar *)"image",      (gchar *)"Input image"                     },
    { GIMP_PDB_DRAWABLE, (gchar *)"drawable",   (gchar *)"Input drawable"                  }
  };

  gimp_install_procedure (PROCEDURE_NAME,
			  "Milena",
			  "Perform many image processing using Milena library",
			  "Olena Team <olena@lrde.epita.fr>",
			  "EPITA Research and Development Laboratory (LRDE)",
			  "2007-2011",
			  N_("Milena toolbox..."),
			  "RGB*, GRAY*, INDEXED*",
			  GIMP_PLUGIN,
			  G_N_ELEMENTS (args), 0,
			  args, NULL);

  gimp_plugin_menu_register (PROCEDURE_NAME, "<Image>/Filters/Milena");
}

static void
run (const gchar      *name,
     gint              n_params,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  (void) name;
  (void) n_params;

  static GimpParam   values[1];
  GimpDrawable      *drawable;
  GimpPixelRgn       in, out;
  gint32             image_ID;
  gint               x1, y1, x2, y2;
  gint		     width, height;
  GimpRunMode        run_mode;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;

  *nreturn_vals = 1;
  *return_vals  = values;

  run_mode = (GimpRunMode) param[0].data.d_int32;
  image_ID = param[1].data.d_int32;
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

  width = drawable->width;
  height = drawable->height;

  gimp_pixel_rgn_init(&out, drawable, 0, 0, width, height, TRUE, TRUE);
  gimp_pixel_rgns_register(1, &out);

  std::cout << "Run - with data: " << out.data << std::endl;

  I tmp(&out);

  //mln::data::fill(tmp, mln::literal::black);
  // mln_piter_(I) p(tmp.domain());
  // for_all(p)
  //   tmp(p) = mln::value::rgb8(0, 0, 0);

  tmp(mln::point2d(0,0)) = mln::value::rgb8(0, 255, 0);
  tmp(mln::point2d(1,0)) = mln::value::rgb8(0, 255, 0);
  tmp(mln::point2d(0,1)) = mln::value::rgb8(0, 255, 0);
  tmp(mln::point2d(1,1)) = mln::value::rgb8(0, 255, 0);

  mln::io::ppm::save(tmp, "/tmp/gimp_image.ppm");
  //   std::cout <<  ".";
  // std::cout << std::endl;


  // build_milena_image(&in, & out);

  // if (strcmp (name, PROCEDURE_NAME) == 0)
  // {
  //   switch (run_mode)
  //   {
  //     case GIMP_RUN_NONINTERACTIVE:
  // 	break;

  //     case GIMP_RUN_INTERACTIVE:
  //     	dialog (image_ID, drawable);
  // 	break;

  //     case GIMP_RUN_WITH_LAST_VALS:
  // 	break;

  //     default:
  // 	break;
  //   }
  // }
  // else
  // {
  //   status = GIMP_PDB_CALLING_ERROR;
  // }

  // gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));

  // gimp_displays_flush ();
  // gimp_drawable_detach (drawable);

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
}
