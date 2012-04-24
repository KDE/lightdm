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
#include <faceimageprovider.h>

#include <QLightDM/UsersModel>

#include <KDebug>
#include <KIconLoader>

#include <QIcon>
#include <QPixmap>

FaceImageProvider::FaceImageProvider(QAbstractItemModel* model)
: QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
, m_model(model)
{
}

QPixmap FaceImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    // Lookup user in model
    QModelIndex userIndex;
    for (int row = 0; row < m_model->rowCount(QModelIndex()); ++row) {
        QModelIndex index = m_model->index(row, 0);
        if (index.data(QLightDM::UsersModel::NameRole).toString() == id) {
            userIndex = index;
            break;
        }
    }
    if (!userIndex.isValid()) {
        kWarning() << "Couldn't find user" << id << "in UsersModel";
        return QPixmap();
    }

    // Get user face pixmap
    QPixmap pix;
    int extent = requestedSize.isValid() ? requestedSize.width() : KIconLoader::SizeEnormous;
    QIcon icon = userIndex.data(Qt::DecorationRole).value<QIcon>();
    if (!icon.isNull()) {
        pix = icon.pixmap(extent);
    }
    if (pix.isNull()) {
        pix = DesktopIcon("user-identity", extent);
    }

    if (size) {
        *size = pix.size();
    }
    if (requestedSize.isValid()) {
        pix = pix.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return pix;
}
