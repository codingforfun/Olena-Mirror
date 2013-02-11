#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "selection.h"
#include "polygonitem.h"

class Scene :
        public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        explicit Scene(const QRectF &sceneRect, QObject *parent = 0);
        explicit Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
        ~Scene();
        void removeItems();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void init();

        Selection *selection;
        QPointF pressPos;
        bool isPressing;
        bool clic;

    signals:
       void repaintItem(const QRectF& rect, bool clic);
};

#endif // SCENE_H
