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
#include <extrarowproxymodel.h>

#include <KDebug>
#include <QAbstractItemModel>

ExtraRowProxyModel::ExtraRowProxyModel(QObject* parent)
: QAbstractListModel(parent)
{}

int ExtraRowProxyModel::appendRow()
{
    Row row;
    m_rows.append(row);
    return m_rows.count() - 1;
}

void ExtraRowProxyModel::setRowData(int id, int column, const QVariant& value, int role)
{
    Q_ASSERT(id < m_rows.count());
    m_rows[id][column][role] = value;
}

void ExtraRowProxyModel::setRowText(int id, int column, const QVariant& value)
{
    setRowData(id, column, value, Qt::DisplayRole);
}

void ExtraRowProxyModel::setSourceModel(QAbstractItemModel* model)
{
    if (! m_model.isNull()) {
        disconnect(m_model.data(), SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(onSourceRowsInserted(QModelIndex,int,int)));
        disconnect(m_model.data(), SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(onSourceRowsRemoved(QModelIndex,int,int)));
        disconnect(m_model.data(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(onSourceDataChanged(QModelIndex,QModelIndex)));
    }

    m_model = QWeakPointer<QAbstractItemModel>(model);
    reset();
    setRoleNames(m_model.data()->roleNames());

    connect(m_model.data(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(onSourceRowsInserted(QModelIndex,int,int)));
    connect(m_model.data(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(onSourceRowsRemoved(QModelIndex,int,int)));
    connect(m_model.data(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onSourceDataChanged(QModelIndex,QModelIndex)));
}

int ExtraRowProxyModel::rowCount(const QModelIndex &) const
{
    if (m_model.isNull()) {
        return m_rows.count();
    } else {
        return m_model.data()->rowCount() + m_rows.count();
    }

}

QVariant ExtraRowProxyModel::data(const QModelIndex &index, int role) const
{
    int count = 0;
    if (!m_model.isNull()) {
        count = m_model.data()->rowCount();
        if (index.row() < count) {
            return m_model.data()->data(index, role);
        }
    }

    int extraRow = index.row() - count;
    if (extraRow >= m_rows.count()) {
        return QVariant();
    }

    const Item& item = m_rows[extraRow][index.column()];
    return item.value(role);
}

void ExtraRowProxyModel::onSourceRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), start, end);
    endInsertRows();
}

void ExtraRowProxyModel::onSourceRowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), start, end);
    endRemoveRows();
}

void ExtraRowProxyModel::onSourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    dataChanged(createIndex(topLeft.row(), 0)   , createIndex(bottomRight.row(), 0));
}


