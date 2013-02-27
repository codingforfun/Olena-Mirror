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

#ifndef PAGESMODEL_HH
# define PAGESMODEL_HH

#include <QAbstractListModel>
#include <QIcon>
#include <QList>

class PagesModel:
        public QAbstractListModel
{
        Q_OBJECT

    public:
        explicit PagesModel(QObject *parent = 0);

        QStringList filenames() const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;

        int currentRow() const;
        void setCurrentRow(int currentRow);

        QVariant data(const QModelIndex& index, int role = Qt::DecorationRole) const;
        void addPicture(const QString& filename, const QPixmap& pixmap);

    private:
        QStringList filenames_;
        QList<QPixmap> pixmaps_;
        int currentRow_;

    public slots:
        void removePixmap(const QModelIndex& parent);
};


#endif // PAGESMODEL_HH
