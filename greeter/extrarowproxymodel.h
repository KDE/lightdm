/*
This file is part of LightDM-KDE.

Copyright 2011, 2012 David Edmundson <kde@davidedmundson.co.uk>

LightDM-KDE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LightDM-KDE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LightDM-KDE.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef EXTRAROWPROXYMODEL_H
#define EXTRAROWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QHash>
#include <QVector>
#include <QWeakPointer>

/**
 * A proxy model which makes it possible to append extra rows at the end
 */
class ExtraRowProxyModel : public QAbstractListModel
{
public:
    ExtraRowProxyModel(QObject *parent = 0);

    /**
     * Append a row, returns a row id
     */
    int appendRow();

    void setRowData(int id, int column, const QVariant &value, int role);
    void setRowText(int id, int column, const QVariant &value);

    int rowCount(const QModelIndex &parent = QModelIndex()) const; // reimp
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; // reimp

    void setSourceModel(QAbstractItemModel* model);

private:
    typedef QHash<int, QVariant> Item;
    typedef QVector<Item> Row;
    typedef QVector<Row> Rows;

    QWeakPointer<QAbstractItemModel> m_model;
    Rows m_rows;
};

#endif /* EXTRAROWPROXYMODEL_H */
