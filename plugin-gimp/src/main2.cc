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

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "interface.hh"

#include "plugin-intl.hh"

#include "build-image.hh"



int main(int argc, char *argv[])
{

  // Gimp* g = gimp_new("Gimp", "test", false, true, true,
  // 		     true, true, true,
  // 		     GIMP_STACK_TRACE_QUERY, GIMP_PDB_COMPAT_ON);

  int width = 50, height = 50;

  gint32 new_image_id = gimp_image_new(width,
				       height,
				       GIMP_RGB);
  /* create new layer */
  gint32 new_layer_id = gimp_layer_new(new_image_id,
				       "titi",
				       width, height,
				       GIMP_RGB_IMAGE,
				       100,
				       GIMP_NORMAL_MODE);
  gimp_image_add_layer(new_image_id, new_layer_id, 0);
  GimpDrawable *new_drawable = gimp_drawable_get(new_layer_id);
  GimpPixelRgn new_region;
  gint32 x1, y1, x2, y2;
  gimp_pixel_rgn_init(&new_region, new_drawable, x1, y1, x2, y2, TRUE, TRUE);

  I tmp(&new_region);
  mln_piter_(I) p(tmp.domain());
  for_all(p)
    tmp(p) = mln::value::rgb8(255, 0, 0);

  mln::io::ppm::save(tmp, "/tmp/gimp_image.ppm");

  gimp_drawable_flush(new_drawable);
  gimp_drawable_merge_shadow(new_drawable->drawable_id, TRUE);
  gimp_drawable_update(new_drawable->drawable_id, x1, y1, x2, y2);
  gimp_drawable_detach(new_drawable);

  /* free buffer */
  gimp_displays_flush();

}
