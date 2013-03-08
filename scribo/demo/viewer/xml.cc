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

#include "xml.hh"

Xml::Xml(const QString& filename)
{
    load(filename);
}

QString Xml::makeXmlFilename(const QString& filename)
{
    QString xml_file = filename;

    // Get xml filename from image path.
    xml_file.remove(0, xml_file.lastIndexOf('/')+1);
    int pos = xml_file.lastIndexOf('.');
    xml_file.remove(pos, xml_file.length()-pos);
    xml_file += "_gui.xml";

    return xml_file;
}

QString Xml::getPath(const QString& filename)
{
    Configs *const configs = Configs::getInstance();

    // Get directory where XML files are stored.
    QFileInfo file(filename);
    QString output_dir = QDir::tempPath();
    if (configs->generalSaveXmlEnabled())
    {
        if (configs->generalSaveXmlSameDir())
            output_dir = file.absolutePath();
        else if (configs->generalSaveXmlCustomDir())
            output_dir = configs->generalSaveXmlCustomDirPath();

        QDir dir(output_dir);
        if (!dir.exists() && !dir.mkpath(output_dir))
            output_dir = QDir::tempPath();
    }

    return output_dir;
}

void Xml::isRecognized(const QDomElement& textElement)
{
    QDomElement lineElement = textElement.firstChild().nextSiblingElement("Line");

    // Check if the attributes "text" exists in the attributes of line in the xml file.
    isRecognized_ = lineElement.attributeNode("text").isNull() ? false : true;
}

void Xml::load(const QString& filename)
{
    if(filename.isEmpty())
    {
        xmlItem_ = 0;
        graphicsItem_ = 0;
        filename_ = QString();
        isRecognized_ = false;
        return;
    }

    // Reset graphic and tree items;
    xmlItem_ = new XmlItem;
    graphicsItem_ = new RootGraphicsItem(13);
    filename_ = getPath(filename);

    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);

    // Fill new xml tree.
    xml_.clear();
    xml_.setContent(&xmlFile);

    xmlFile.close();

    QDomElement root = xml_.documentElement();
    xmlItem_->load(root);
    xmlItem_->show();

    root = root.firstChild().toElement();
    XmlItem *parentTreeItem = init(root, xmlItem_);

    // Run through the xml file structure by structure.
    root = root.nextSibling().firstChild().toElement();
    // Check if the xml file has been recognized by the ocr or not.
    isRecognized(root);

    processNode(root, datas_[0], parentTreeItem);
    for(int i = 2; i < 11; i++)
        processNode(root.nextSiblingElement(datas_[i].name), datas_[i], parentTreeItem);
}

XmlItem *Xml::init(const QDomElement& root, XmlItem *rootTreeItem)
{
    XmlItem *metadataItem = new XmlItem(root, rootTreeItem);

    XmlItem *contentItem = 0;

    // Run through "METADATA" node and subnodes.
    QDomElement node = root.firstChild().toElement();
    while(!node.isNull())
    {
        contentItem = new XmlItem(node, metadataItem, contentItem);
        node = node.nextSibling().toElement();
    }

    // Add "PAGE" root node and return it.
    metadataItem = new XmlItem(root.nextSibling().toElement(), rootTreeItem, metadataItem);
    metadataItem->show();

    return metadataItem;
}

void Xml::processNode(const QDomElement& root, const GraphicsRegion::Data& data, XmlItem *pageItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *regionItem = new XmlItem(root, pageItem);

        QDomElement node = root.firstChild().toElement();
        XmlItem *coordsItem = new XmlItem(node, regionItem);

        QPolygon polygon;
        XmlItem *pointItem = 0;
        QString x, y;

        // Run through all points data.
        node = node.firstChild().toElement();
        while(!node.isNull())
        {
            x = node.attribute("x", "null");
            y = node.attribute("y", "null");

            pointItem = new XmlItem(node, coordsItem, pointItem);

            polygon << QPoint(x.toInt(), y.toInt());
            node = node.nextSibling().toElement();
        }

        // Create the graphics item from data structure.
        RegionItem *graphicsItem = new RegionItem(polygon, graphicsItem_);
        graphicsItem->loadData(data);
        graphicsItem_->addItemFrom(graphicsItem, data.region);

        // Store xml item and graphics item in each object for selection.
        regionItem->setRegionItem(graphicsItem);
        graphicsItem->setXmlItem(regionItem);

        if(data.region == GraphicsRegion::TextRegion)
            processLineNode(root.firstChild().nextSiblingElement("Line"), coordsItem);

        // Run through all nodes from the same data structure recursively.
        processNode(root.nextSiblingElement(data.name), data, pageItem);
    }
}

void Xml::processLineNode(const QDomElement& root, XmlItem *precItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *lineItem = new XmlItem(root, precItem->parent(), precItem);

        QDomElement node = root.firstChild().toElement();
        XmlItem *coordsItem = new XmlItem(node, lineItem);

        QPolygon polygon;
        XmlItem *pointItem = 0;

        QString xs, ys;
        int x, y;

        int xMin = INT_MAX;
        int xMax = INT_MIN;

        // Run through all points data.
        node = node.firstChild().toElement();
        while(!node.isNull())
        {
            xs = node.attribute("x", "null");
            ys = node.attribute("y", "null");

            pointItem = new XmlItem(node, coordsItem, pointItem);

            x = xs.toInt();
            y = ys.toInt();

            // Get beginning and end of line.
            if(xMin > x)
                xMin = x;
            if(xMax < x)
                xMax = x;

            polygon << QPoint(x, y);
            node = node.nextSibling().toElement();
        }

        // Create the graphics item from data structure.
        RegionItem *graphicsItem = new RegionItem(polygon, graphicsItem_);
        graphicsItem->loadData(datas_[1]);
        graphicsItem_->addItemFrom(graphicsItem, datas_[1].region);

        // Store xml item and graphics item in each object for selection.
        lineItem->setRegionItem(graphicsItem);
        graphicsItem->setXmlItem(lineItem);

        // Get meanline and baseline.
        processTypoNode(root, QPoint(xMin, xMax));
        // Run through all line nodes recursively.
        processLineNode(root.nextSiblingElement(datas_[1].name), lineItem);
    }
}

void Xml::processTypoNode(const QDomElement& root, const QPoint& xPoint)
{
    int yPos = root.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)));
    baselineItem->setZValue(datas_[11].zValue);
    graphicsItem_->addItemFrom(baselineItem, datas_[11].region);

    yPos = root.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)));
    meanlineItem->setZValue(datas_[12].zValue);
    graphicsItem_->addItemFrom(meanlineItem, datas_[12].region);
}


XmlItem *Xml::xmlItem()
{
  return xmlItem_;
}

RootGraphicsItem *Xml::graphicsItem()
{
  return graphicsItem_;
}

QDomDocument Xml::document() const
{
  return xml_;
}

QString Xml::filename() const
{
  return filename_;
}

GraphicsRegion::Data Xml::dataFromRegion(GraphicsRegion::Id region)
{
  return datas_[region];
}

bool Xml::recognized() const
{
  return isRecognized_;
}
