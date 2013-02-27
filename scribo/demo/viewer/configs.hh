// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef CONFIGS_H
# define CONFIGS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>
#include <QSettings>
#include <QDir>
#include <QDebug>

#include "region.hh"

class Configs :
  public QSettings
{
public:
  static Configs *getInstance();

  bool preprocessingSubsample();
  void setPreprocessingSubsample(bool b);

  bool preprocessingRemoveBg();
  void setPreprocessingRemoveBg(bool b);

  bool preprocessingDeskew();
  void setPreprocessingDeskew(bool b);

  bool preprocessingRemoveNoise();
  void setPreprocessingRemoveNoise(bool b);

  int preprocessingBinAlgo();
  void setPreprocessingBinAlgo(int algo);

  int segmentationFindSeps();
  void setSegmentationFindSeps(int seps);

  bool ocrEnabled();
  void setOcrEnabled(bool b);

  QString ocrLanguage();
  void setOcrLanguage(const QString& lang);

  bool generalSaveXmlEnabled();
  void setGeneralSaveXmlEnabled(bool b);

  bool generalSaveXmlSameDir();
  void setGeneralSaveXmlSameDir(bool b);

  bool generalSaveXmlCustomDir();
  void setGeneralSaveXmlCustomDir(bool b);

  QString generalSaveXmlCustomDirPath();
  void setGeneralSaveXmlCustomDirPath(const QString& path);

private:
  explicit Configs();
};


#endif // CONFIGS_H
