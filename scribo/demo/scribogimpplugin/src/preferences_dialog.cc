// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#include <scribo/core/document.hh>

#include "preferences_dialog.hh"
#include "general_options.hh"
#include "ocr_options.hh"
#include "preprocessing_options.hh"
#include "segmentation_options.hh"

// FIXME
#include <mln/value/qt/rgb32.hh>
#include <mln/data/convert.hh>
#include <mln/data/paste_without_localization.hh>
#include <mln/core/image/dmorph/image_if.hh>


preferences_dialog::preferences_dialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  // We may want to remove this useless object in the ui file.
  delete widget;

  setAttribute(Qt::WA_DeleteOnClose);

  load_option_list();
  optionList->setCurrentRow(0);

  // Progress dialog setup
  pdialog_.setModal(true);
  pdialog_.setAutoClose(false);
  pdialog_.setCancelButton(0);
  connect(&runner_, SIGNAL(finished()),
	  &pdialog_, SLOT(close()));
  connect(&runner_, SIGNAL(finished()),
	  this, SLOT(diplay_result()));
  connect(&runner_, SIGNAL(new_step(const QString&)),
	  &pdialog_, SLOT(setWindowTitle(const QString&)));
  connect(&runner_, SIGNAL(new_progress_max_value(int)),
	  &pdialog_, SLOT(setMaximum(int)));
  connect(&runner_, SIGNAL(new_progress_label(const QString&)),
	  &pdialog_, SLOT(setLabelText(const QString&)));

  // Runner setup
  connect(&runner_, SIGNAL(progress(int)),
	  &pdialog_, SLOT(setValue(int)));

}

preferences_dialog::~preferences_dialog()
{
  for (int i = 0; i < widgets_.size(); ++i)
    if (widgets_[i])
    {
      static_cast<OptionWidget *>(widgets_[i])->save_config();
      delete widgets_[i];
    }
}


void preferences_dialog::set_current_image(const mln::image2d<mln::value::rgb8>& ima, gint32 image_id)
{
  current_image_ = ima;
  image_id_ = image_id;
}


void preferences_dialog::load_option_list()
{
  static const char *options[] = { "General", "Preprocessing", "Segmentation", "OCR", 0 };

  int i;
  for (i = 0; options[i]; ++i)
    optionList->insertItem(i, options[i]);

  widgets_.fill(0, i);
}

void preferences_dialog::on_optionList_currentRowChanged(int row)
{
  select_option_widget(row);
}


void preferences_dialog::select_option_widget(int row)
{
  if (row >= widgets_.size())
  {
    qDebug() << "select_option_widget - Hu? Something wrong... Invalid row";
    return;
  }

  if (!widgets_[row])
  {
    QWidget *widget = 0;

    switch (row)
    {
      case 0:
	widget = new general_options(this);
	break;

      case 1:
	widget = new preprocessing_options(this);
	break;

      case 2:
	widget = new segmentation_options(this);
	break;

      case 3:
	widget = new ocr_options(this);
	break;

      default:
	qDebug() << "select_option_widget - Hu? Something wrong...";
    }

    if (widget)
      widgets_[row] = widget;
  }

  if (horizontalLayout_2->count() == 2)
  {
    QWidget *current_widget = horizontalLayout_2->itemAt(1)->widget();
    horizontalLayout_2->removeWidget(current_widget);
    current_widget->hide();
  }

  horizontalLayout_2->insertWidget(1, widgets_[row]);
  widgets_[row]->show();
}


void preferences_dialog::accept()
{
  // for (int i = 0; i < widgets_.size(); ++i)
  //   if (widgets_[i])
  //   {
  //     static_cast<OptionWidget *>(widgets_[i])->save_config();
  //     delete widgets_[i];
  //   }
  hide();
  process();
//  QDialog::accept();
}

void preferences_dialog::process()
{
  // Run process ?
  reset_progress_dialog();
  std::cout << "Starting demat" << std::endl;
  runner_.start_demat(current_image_);
  std::cout << "Ending demat" << std::endl;
}




void preferences_dialog::reject()
{
  for (int i = 0; i < widgets_.size(); ++i)
    delete widgets_[i];
  QDialog::reject();
}

void preferences_dialog::reset_progress_dialog()
{
  pdialog_.setValue(0);
  pdialog_.setLabelText("");
  pdialog_.show();
}

void preferences_dialog::diplay_result()
{
  std::cout << "Creating result image" << std::endl;

  // Make sure there is not border in input image.
  mln::border::resize(current_image_, 0);

  // Getting retrieved document.
  const scribo::document<L>& doc = runner_.result();

  image2d<value::qt::rgb32> in_32 = data::convert(value::qt::rgb32(), current_image_);

  int
    width = current_image_.bbox().width(),
    height = current_image_.bbox().height();

  // Get active drawable in input image.
  gint32 drawable_id = gimp_image_get_active_drawable(image_id_);

  // Get coordinates of the processed area in the input image.
  gint32 x1, y1, x2, y2;
  gimp_drawable_mask_bounds(drawable_id, &x1, &y1, &x2, &y2);

  // Create a new image
  gint32 new_image_id = gimp_image_new(width,
  				       height,
  				       gimp_image_base_type(image_id_));

    /* create background layer */
  gint32 new_layer_id = gimp_layer_new(new_image_id,
				       "Background",
				       width, height,
				       gimp_drawable_type(drawable_id),
				       100,
				       GIMP_NORMAL_MODE);
  gimp_image_add_layer(new_image_id, new_layer_id, 0);


  if (doc.has_text())
  {
    /* create new layer */
    gint32 new_layer_id = gimp_layer_new(new_image_id,
					 "Text",
					 width, height,
					 gimp_drawable_type(drawable_id),
					 100,
					 GIMP_NORMAL_MODE);
    gimp_image_add_layer(new_image_id, new_layer_id, -1);
    gimp_layer_add_alpha(new_layer_id);

    GimpDrawable *new_drawable = gimp_drawable_get(new_layer_id);
    GimpPixelRgn new_region;
    gimp_pixel_rgn_init(&new_region, new_drawable, x1, y1, x2, y2, TRUE, TRUE);

    for_all_lines(i, doc.lines())
      if (doc.lines()(i).is_textline())
      {
	std::cout << "Drawing text with bbox " << doc.lines()(i).bbox() << std::endl;
	const scribo::line_info<L>& line = doc.lines()(i);

	// FIXME: Slow!  gimp_pixel_rgn_set_rect does not support
	// stride information thus, requires that relevant data in the
	// buffer is contiguous.
	mln::image2d<value::qt::rgb32> tmp(line.bbox(), 0);
	mln::data::paste_without_localization(in_32 | line.bbox(), tmp);

	gimp_pixel_rgn_set_rect(&new_region,
				(const guchar *) tmp.buffer(),
				line.bbox().pmin().col(),
				line.bbox().pmin().row(),
				line.bbox().width(),
				line.bbox().height());
      }

    /* Tidy up dirty drawable */
    gimp_drawable_flush(new_drawable);
    gimp_drawable_merge_shadow(new_drawable->drawable_id, TRUE);
    gimp_drawable_update(new_drawable->drawable_id, x1, y1, x2, y2);
    gimp_drawable_detach(new_drawable);

    gimp_displays_flush();
  }


  if (doc.has_whitespace_seps())
    std::cout << "doc has whitespace seps!" << std::endl;


  if (doc.has_elements())
  {
    /* create new layer */
    gint32 new_layer_id = gimp_layer_new(new_image_id,
					 "Elements",
					 width, height,
					 gimp_drawable_type(drawable_id),
					 100,
					 GIMP_NORMAL_MODE);
    gimp_image_add_layer(new_image_id, new_layer_id, -1);
    gimp_layer_add_alpha(new_layer_id);

    GimpDrawable *new_drawable = gimp_drawable_get(new_layer_id);
    GimpPixelRgn new_region;
    gimp_pixel_rgn_init(&new_region, new_drawable, x1, y1, x2, y2, TRUE, TRUE);

    for_all_comps(i, doc.elements())
      if (doc.elements()(i).is_valid())
      {
	std::cout << "Drawing image with bbox " << doc.elements()(i).bbox() << std::endl;
	const scribo::component_info<L>& elt = doc.elements()(i);

	// FIXME: Slow!  gimp_pixel_rgn_set_rect does not support
	// stride information thus, requires that relevant data in the
	// buffer is contiguous.
	mln::image2d<value::qt::rgb32> tmp(elt.bbox(), 0);
	mln::data::paste_without_localization(in_32 | elt.bbox(), tmp);

	gimp_pixel_rgn_set_rect(&new_region,
				(const guchar *) tmp.buffer(),
				elt.bbox().pmin().col(),
				elt.bbox().pmin().row(),
				elt.bbox().width(),
				elt.bbox().height());
      }

    /* Tidy up dirty drawable */
    gimp_drawable_flush(new_drawable);
    gimp_drawable_merge_shadow(new_drawable->drawable_id, TRUE);
    gimp_drawable_update(new_drawable->drawable_id, x1, y1, x2, y2);
    gimp_drawable_detach(new_drawable);

    gimp_displays_flush();
  }

  /* free buffer */
  gimp_display_new(new_image_id);
}
