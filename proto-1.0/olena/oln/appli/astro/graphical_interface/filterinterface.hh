// Copyright (C) 2005  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef _FILTERINTERFACE_HH_
# define _FILTERINTERFACE_HH_
# include <string>
# include <oln/appli/astro/clean.hh>
# include "qtincludes.hh"
# include "visualization_window.h"
# include "image_viewer.hh"
# include "utils.hh"

class FilterVisualizationWindow : public visualisation_window
{

public:
  
  typedef oln::image_with_nbh<oln::image2d<ntg::int_u8>, oln::neighborhood2d> ima_type;
  
  FilterVisualizationWindow(const std::string& input_file_,
			    oln::appli::astro::clean<ima_type>& mt)
    : mt_(mt), input_file_(input_file_)
  {
    display_in_.reload(input_file_.c_str());
    display_in_.hide();
    display_out_.hide();
    sprintf(tmp_, "/tmp/olni.XXXXXX");
    mkstemp(tmp_);
    sprintf(tmp_ + 16, ".pgm");

    Attribute_area();
    Attribute_level();
    Attribute_height();
    Attribute_tower();
    Attribute_tower();
    Attribute_center_point();
    Attribute_circle();
    Attribute_height();
    Attribute_level();
    Attribute_area();
    UpdateValues();
  }

  ~FilterVisualizationWindow()
  {
    unlink(tmp_);
  }

  virtual void UpdateValues()
  {
    mt_.set_area_min(value_area_min->value());
    mt_.set_area_max(value_area_max->value());
    mt_.set_level(value_level->value());
    mt_.set_height(value_height->value());
    mt_.set_circle(value_circle->value() / 100);
    mt_.set_tower(value_tower->value() / 100);

    std::string out = "attributes value changed.";//  area("
//       + Utils::int2string(value_area->value())
//       + "), point_value("
//       + Utils::int2string(value_level->value())
//       + ")";
    this->text_state->setText(out.c_str());
  }

  virtual void ComputeTree()
  {
    this->text_state->setText("computing tree, please wait...");
    this->compute_button->setEnabled(false);

    mt_.compute_tree();

    this->redraw_button->setEnabled(true);
    this->save_result_button->setEnabled(true);
    this->text_state->setText("Tree computed, please choose attributes'values to draw the result.");
  }
  
  virtual void DisplayImage()
  {
    this->text_state->setText("computing resulting image, please wait...");
    mt_.compute_attributes();
    mt_.process_filter();
    oln::io::write(mt_.output, tmp_);
    this->text_state->setText("displaying result");
    display_out_.reload(tmp_);
    this->text_state->setText("");
    display_out_.show();
  }

  virtual void DisplayInputImage()
  {
    display_in_.show();
  }

  virtual void SaveResult()
  {
    this->text_state->setText("saving resulting image, please wait...");
    oln::io::write(mt_.output, "./out.pgm");
    this->text_state->setText("image succesfully saved to ./out.pgm");
  }

  virtual void Attribute_area()
  {
    if (this->active_area->isChecked())
      {
	this->value_area_min->setEnabled(true);
	this->value_area_max->setEnabled(true);
	mt_.area_tag_ = true;
      }
    else
      {
	this->value_area_min->setEnabled(false);
	this->value_area_max->setEnabled(false);
	mt_.area_tag_ = false;
      }
  }

  virtual void Attribute_level()
  {
    if (this->active_level->isChecked())
      {
	this->value_level->setEnabled(true);
	mt_.level_tag_ = true;
      }
    else
      {
	this->value_level->setEnabled(false);
	mt_.level_tag_ = false;
      }
  }

  virtual void Attribute_height()
  {
    if (this->active_height->isChecked())
      {
	this->value_height->setEnabled(true);
	mt_.height_tag_ = true;
      }
    else
      {
	this->value_height->setEnabled(false);
	mt_.height_tag_ = false;
      }
  }

  virtual void Attribute_circle()
  {
    if (this->active_circle->isChecked())
      {
	this->value_circle->setEnabled(true);
	mt_.circle_tag_ = true;
      }
    else
      {
	mt_.circle_tag_ = false;
	this->value_circle->setEnabled(false);
      }
  }

  virtual void Attribute_center_point()
  {
    if (this->active_center_point->isChecked())
      mt_.center_p_tag_ = true;
    else
      mt_.center_p_tag_ = false;
  }

  virtual void Attribute_tower()
  {
    if (this->active_tower->isChecked())
      {
	this->value_tower->setEnabled(true);
	mt_.tour_tag_ = true;
      }
    else
      {
	mt_.tour_tag_ = false;
	this->value_tower->setEnabled(false);
      }
  }

protected:
  oln::appli::astro::clean<ima_type>& mt_;
  image_viewer display_in_;
  image_viewer display_out_;
  std::string input_file_;
  char tmp_[21];
};

#endif // !_FILTERINTERFACE_HH_
