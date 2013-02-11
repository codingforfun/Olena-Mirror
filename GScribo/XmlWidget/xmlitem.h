#ifndef XMLITEM_H
#define XMLITEM_H

#include <QDomElement>

#include "xmlattributes.h"

class PolygonItem;

class XmlItem
{
    public:
        explicit XmlItem(XmlItem *parent = 0, XmlItem *precItem = 0);
        explicit XmlItem(const QDomElement& node, XmlItem *parent = 0, XmlItem *precItem = 0);
        ~XmlItem();

        void load(const QDomElement& node);

        inline QList<XmlItem *> childs() const;
        inline XmlItem *child(int i) const;
        inline void addChild(XmlItem *child);
        inline XmlItem *parent() const;

        inline PolygonItem *graphicalItem();
        inline void setGraphicalItem(PolygonItem *graphicalItem);

        inline bool isVisible() const;
        inline void setVisible(bool visible);
        inline void show();
        inline void hide();

        inline QString text() const;
        inline int row() const;
        inline XmlAttributes attributes() const;

    private:
        void init(XmlItem *parent = 0, XmlItem *precItem = 0);

        int row_;
        bool isVisible_;
        QList<XmlItem *> childs_;
        XmlItem *parent_;
        PolygonItem *graphicalItem_;
        QString text_;
        XmlAttributes attributes_;
};

inline QList<XmlItem *> XmlItem::childs() const
{ return childs_; }

inline XmlItem *XmlItem::child(int i) const
{ if(i > -1 && i < childs_.count()) return childs_[i]; return 0; }

inline void XmlItem::addChild(XmlItem *child)
{ childs_ << child; }

inline XmlItem *XmlItem::parent() const
{ return parent_; }

inline PolygonItem *XmlItem::graphicalItem()
{ return graphicalItem_; }

inline void XmlItem::setGraphicalItem(PolygonItem *graphicalItem)
{ graphicalItem_ = graphicalItem; }

inline bool XmlItem::isVisible() const
{ return isVisible_; }

inline void XmlItem::setVisible(bool visible)
{ isVisible_ = visible; }

inline void XmlItem::show()
{ isVisible_ = true; }

inline void XmlItem::hide()
{ isVisible_ = false; }

inline QString XmlItem::text() const
{ return text_; }

inline int XmlItem::row() const
{ return row_; }

inline XmlAttributes XmlItem::attributes() const
{ return attributes_; }

#endif // XMLITEM_H
