#ifndef FACEIMAGEPROVIDER_H
#define FACEIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

namespace QLightDM
{
    class UsersModel;
}

class QAbstractItemModel;

class FaceImageProvider : public QDeclarativeImageProvider
{
public:
    FaceImageProvider(QAbstractItemModel*);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QAbstractItemModel* m_model;
};

#endif
