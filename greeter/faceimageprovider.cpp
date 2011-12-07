#include <faceimageprovider.h>

#include <QLightDM/UsersModel>

#include <KDebug>
#include <KIconLoader>

#include <QPixmap>

FaceImageProvider::FaceImageProvider(QLightDM::UsersModel* model)
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
    QPixmap pix = userIndex.data(Qt::DecorationRole).value<QPixmap>();
    if (pix.isNull()) {
        pix = DesktopIcon("user-identity");
    }
    if (size) {
        *size = pix.size();
    }
    if (requestedSize.isValid()) {
        pix = pix.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return pix;
}
