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
    reset();
    loadScreens();
}

void ScreensModel::onScreenCountChanged(int newCount)
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
