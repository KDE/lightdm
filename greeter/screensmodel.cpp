/*
This file is part of LightDM-KDE.

Copyright 2012 David Edmundson <kde@davidedmundson.co.uk>

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

#include "screensmodel.h"

#include <QApplication>
#include <QDesktopWidget>

ScreensModel::ScreensModel(QObject *parent) :
    QAbstractListModel(parent)
{
    loadScreens();
    QDesktopWidget *dw = QApplication::desktop();
    connect(dw, SIGNAL(screenCountChanged(int)), SLOT(onScreenCountChanged(int)));
    connect(dw, SIGNAL(resized(int)), SLOT(onResized(int)));

    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "geometry";
    setRoleNames(roles);
}

int ScreensModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex()) {
        return m_screens.size();
    }
    return 0;
}

QVariant ScreensModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row < 0 || row >= m_screens.size()) {
        return QVariant();
    }

    if (role == Qt::UserRole) {
        return m_screens[row];
    }
    return QVariant();
}

void ScreensModel::onScreenResized(int screen)
{
    QDesktopWidget *dw = QApplication::desktop();

    if (screen >= 0 && screen < m_screens.size()) {
        m_screens[screen] = dw->screenGeometry(screen);
    }
    QModelIndex index = createIndex(screen,0);
    dataChanged(index, index);
}

void ScreensModel::onScreenCountChanged(int newCount)
{
    Q_UNUSED(newCount);
    loadScreens();
}

void ScreensModel::loadScreens()
{
    beginResetModel();
    m_screens.clear();
    QDesktopWidget *dw = QApplication::desktop();
    for (int i=0;i<dw->screenCount();i++) {
        m_screens.append(dw->screenGeometry(i));
    }
    endResetModel();
}
