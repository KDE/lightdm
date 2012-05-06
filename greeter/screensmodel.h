#ifndef SCREENSMODEL_H
#define SCREENSMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QRect>

class ScreensModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ScreensModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private slots:
    void onResized();
    void screenCountChanged();

private:
    void loadScreens();
    QList<QRect> m_screens;
};

#endif // SCREENSMODEL_H
