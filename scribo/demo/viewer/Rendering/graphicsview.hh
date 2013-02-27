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

#ifndef GRAPHICSVIEW_HH
# define GRAPHICSVIEW_HH

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>

class GraphicsView :
        public QGraphicsView
{
        Q_OBJECT

    public:
        explicit GraphicsView(QWidget *parent = 0);
        explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = 0);

    protected:
        void init();

        void resizeEvent(QResizeEvent *event);

        void wheelEvent(QWheelEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        bool isWidthGreater_;
        bool isContainedInView_;
        QSizeF scaleRatio_;

        bool isRightClicking_;
        QPointF prevMousePos_;

    public slots:
        void fitInView(const QRectF& rect);

    signals:
        void beginDrag();
        void endDrag();
};

#endif // GRAPHICSVIEW_HH
