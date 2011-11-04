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

#include <mln/util/timer.hh>
#include <mln/data/fill.hh>
#include <mln/literal/colors.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/paste.hh>

#include <mln/core/image/gimp-image.hh>

//#include "interface.hh"

#include "plugin-intl.hh"

//#include "build-image.hh"

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
//  getchar();
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
			  "2011",
			  N_("Milena toolbox..."),
			  "RGB*, GRAY*",
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
  gint32             image_ID;
  GimpRunMode        run_mode;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;

  *nreturn_vals = 1;
  *return_vals  = values;

  run_mode = (GimpRunMode) param[0].data.d_int32;
  image_ID = param[1].data.d_int32;
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  guint32 valtype = gimp_drawable_type(drawable->drawable_id);
  if (valtype == GIMP_RGB_IMAGE)
  {
    typedef mln::gimp_image<GIMP_RGB_IMAGE> I;
    {
      mln::util::timer t;
      t.start();
      I tmp(drawable);
      t.stop();
      std::cout << "Milena Gimp image built in " << t << "s" << std::endl;
      t.restart();

      mln::data::fill(tmp, mln::literal::red);

      t.stop();
      std::cout << "Gimp image filled in " << t << "s" << std::endl;

      tmp(mln::point2d(0,0)) = mln::value::rgb8(255, 0, 0);
      tmp(mln::point2d(1,0)) = mln::value::rgb8(0, 0, 255);
      tmp(mln::point2d(0,1)) = mln::value::rgb8(0, 255, 0);
      tmp(mln::point2d(1,1)) = mln::value::rgb8(0, 255, 0);
    }
  }
  else if (valtype == GIMP_GRAY_IMAGE)
  {
    typedef mln::gimp_image<GIMP_GRAY_IMAGE> I;
    {
      mln::util::timer t;
      t.start();
      I tmp(drawable);
      t.stop();
      std::cout << "Milena Gimp image built in " << t << "s" << std::endl;
      t.restart();

      mln::data::fill(tmp, 125);

      t.stop();
      std::cout << "Gimp image filled in " << t << "s" << std::endl;

      tmp(mln::point2d(0,0)) = 64;
      tmp(mln::point2d(1,0)) = 150;
      tmp(mln::point2d(0,1)) = 255;
      tmp(mln::point2d(1,1)) = 0;
    }
  }

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
}
