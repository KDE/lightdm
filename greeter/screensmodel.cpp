#include "screensmodel.h"

#include <QApplication>
#include <QDesktopWidget>

ScreensModel::ScreensModel(QObject *parent) :
    QAbstractListModel(parent)
{
    loadScreens();
    QDesktopWidget *dw = QApplication::desktop();
    connect(dw, SIGNAL(screenCountChanged(int)), SLOT(screenCountChanged()));
    connect(dw, SIGNAL(resized(int)), SLOT(onResized()));

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

    Q_ASSERT(row > 0 && row < rowCount());

    if (role == Qt::UserRole) {
        return m_screens[row];
    }
    return QVariant();
}

void ScreensModel::onResized()
{
    reset();
    loadScreens();
}

void ScreensModel::screenCountChanged()
{
    reset();
    loadScreens();
}

void ScreensModel::loadScreens()
{
    m_screens.clear();
    QDesktopWidget *dw = QApplication::desktop();
    for (int i=0;i<dw->screenCount();i++) {
        m_screens.append(dw->screenGeometry(i));
    }
}
