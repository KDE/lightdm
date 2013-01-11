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
#include <QStandardItemModel>

/**
 * A proxy model which makes it possible to append extra rows at the end
 */
class ExtraRowProxyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ExtraRowProxyModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const; // reimp
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; // reimp

    void setSourceModel(QAbstractItemModel* model);

    /** Returns a pointer to the extra row model, which can be edited as appropriate*/
    QStandardItemModel* extraRowModel() const;

private slots:
    void onSourceRowsInserted(const QModelIndex &parent,int start,int end);
    void onSourceRowsRemoved(const QModelIndex &parent,int start,int end);
    void onSourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void onExtraRowsInserted(const QModelIndex &parent,int start,int end);
    void onExtraRowsRemoved(const QModelIndex &parent,int start,int end);
    void onExtraDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);


private:
    int sourceRowCount() const;

    QWeakPointer<QAbstractItemModel> m_model;
    QStandardItemModel *m_extraRowModel;
};

#endif /* EXTRAROWPROXYMODEL_H */
