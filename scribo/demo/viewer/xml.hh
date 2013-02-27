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

#ifndef XML_HH
# define XML_HH

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <QDomAttr>
#include <climits>
#include <QFile>

#include "Rendering/rootgraphicsitem.hh"
#include "Rendering/regionitem.hh"
#include "XmlWidget/xmlitem.hh"
#include "variantpointer.hh"
#include "configs.hh"
#include "region.hh"

// WARNING : The order is VERY IMPORTANT (depending on the enum).
static const GraphicsRegion::Data datas_[] =
{
    /*           COLOR               |             NAME            |                 REGION                |              PARENT             |  Z */
    { QColor::fromRgb(0, 100, 0, 110),   "TextRegion",                   GraphicsRegion::TextRegion,             GraphicsRegion::Text,           1 },
    { QColor::fromRgb(255, 0, 0, 110),   "Line",                         GraphicsRegion::Line,                   GraphicsRegion::Text,           2 },
    { QColor::fromRgb(0, 0, 255, 110),   "VerticalSeparatorRegion",      GraphicsRegion::VerticalSeparator,      GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(0, 0, 255, 110),   "HorizontalSeparatorRegion",    GraphicsRegion::HorizontalSeparator,    GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(0, 0, 128, 110),   "WhitespaceSeparatorRegion",    GraphicsRegion::WhiteSpaceSeparator,    GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(255, 120, 0, 110), "ImageRegion",                  GraphicsRegion::Image,                  GraphicsRegion::Miscellaneous,  1 },
    { QColor::fromRgb(43, 39, 128, 110), "NoiseRegion",                  GraphicsRegion::Noise,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(220, 246, 0, 110), "TableRegion",                  GraphicsRegion::Table,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(170, 0, 255, 110), "MathsRegion",                  GraphicsRegion::Maths,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(255, 0, 144, 110), "GraphicsRegion",               GraphicsRegion::Graphic,                GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(0, 204, 255, 110), "ChartRegion",                  GraphicsRegion::Chart,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(128, 0, 255),     "Baseline",                     GraphicsRegion::Baseline,               GraphicsRegion::Typology,       3 },
    { QColor::fromRgb(128, 0, 255),     "Meanline",                     GraphicsRegion::Meanline,               GraphicsRegion::Typology,       3 },
    { QColor::fromRgb(255, 255, 255),   "Text",                         GraphicsRegion::Text,                   GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Separators",                   GraphicsRegion::Separators,             GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Miscellaneous",                GraphicsRegion::Miscellaneous,          GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Typological Lines",            GraphicsRegion::Typology,               GraphicsRegion::None,           0 }
};

class Xml
{
public:
  explicit Xml(const QString& filename = QString());

  XmlItem *xmlItem();
  RootGraphicsItem *graphicsItem();

  QDomDocument document() const;
  QString filename() const;
  bool recognized() const;

  static QString getPath(const QString& filename);

  void load(const QString& filename);
  static GraphicsRegion::Data dataFromRegion(GraphicsRegion::Id region);

private:
  XmlItem *init(const QDomElement& root, XmlItem *rootTreeItem);
  void isRecognized(const QDomElement& element);

  void processNode(const QDomElement& root, const GraphicsRegion::Data& data, XmlItem *rootTreeItem);
  void processLineNode(const QDomElement& root, XmlItem *rootTreeItem);
  void processTypoNode(const QDomElement& root, const QPoint& xPoint);

  RootGraphicsItem *graphicsItem_;
  XmlItem *xmlItem_;

  QDomDocument xml_;
  QString filename_;
  bool isRecognized_;
};

#endif // XML_HH
