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

#include "runner.hh"

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>

#include "process.hh"
#include "preprocess.hh"
#include "config.hh"
#include "defs.hh"



using namespace mln;
using namespace scribo::toolchain::internal;


runner::runner(QObject *parent)
  : QThread(parent)
{
  moveToThread(this);
}


void runner::run()
{

  switch(mode_)
  {
    default:
    case Demat:
    {
      image2d<bool> bin_ima = preprocess(args_.at(0));
      process(args_.at(0), bin_ima);
    }
    break;
  }

  emit finished();
  qDebug() << "Done.";
}


void runner::stop()
{
  terminate();
}



// Demat related stuff

void
runner::start_demat(const image2d<value::rgb8>& image)
{
  args_.clear();
  args_ << image;
  mode_ = Demat;

  QThread::start();
}

const runner::doc_t&
runner::result() const
{
  return doc_;
}

void
runner::progress_value()
{
  emit progress(++step_);
}

image2d<bool>
runner::preprocess(const image2d<value::rgb8>& ima)
{
  emit new_step("Preprocessing");

  ::preprocess f;

  // Load config.
  config * const conf = config::get_instance();

  // Init runner and functor connections.
  step_ = 0;
  connect(&f, SIGNAL(progress()), this, SLOT(progress_value()));
  connect(&f, SIGNAL(new_progress_label(const QString&)),
	  this, SIGNAL(new_progress_label(const QString&)));

  f.enable_subsample = conf->preprocessing_subsample();
  f.enable_fg_extraction = conf->preprocessing_remove_bg();
  f.enable_deskew = conf->preprocessing_deskew();
  f.enable_denoising = conf->preprocessing_remove_noise();

  f.binarization_algo = static_cast<Binarization_Algo>(conf->preprocessing_bin_algo());

  emit new_progress_max_value(f.nsteps());

  // Perform preprocessing.
  f(ima);

  qDebug() << "Preprocess Done.";
  return f.output;
}


void runner::process(const image2d<value::rgb8>& original_ima,
		     const image2d<bool>& processed_ima)
{
  emit new_step("Page segmentation");

  ::process f("fixme.jpg");

  // Init runner and functor connections.
  step_ = 0;
  connect(&f, SIGNAL(progress()), this, SLOT(progress_value()));
  connect(&f, SIGNAL(new_progress_label(const QString&)),
	  this, SIGNAL(new_progress_label(const QString&)));

  // Load config.
  config * const conf = config::get_instance();

  defs::FindSeparators
    find_seps = static_cast<defs::FindSeparators>(conf->segmentation_find_seps());
  f.enable_line_seps = (find_seps == defs::Lines
			|| find_seps == defs::LinesAndWhitespaces);
  f.enable_whitespace_seps = (find_seps == defs::Whitespaces
			      || find_seps == defs::LinesAndWhitespaces);
  f.enable_ocr = conf->ocr_enabled();
  f.ocr_language = conf->ocr_language().toAscii().data();
  f.save_doc_as_xml = false;

  emit new_progress_max_value(f.nsteps());

  // Perform text detection.
  f(original_ima, processed_ima);

  doc_ = f.doc;

  qDebug() << "Process Done.";
}


