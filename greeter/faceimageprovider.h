#ifndef FACEIMAGEPROVIDER_H
#define FACEIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

namespace QLightDM
{
    class UsersModel;
}

class FaceImageProvider : public QDeclarativeImageProvider
{
public:
    FaceImageProvider(QLightDM::UsersModel*);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QLightDM::UsersModel* m_model;
};

#endif
