#include "modelcombobox.h"

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QDebug>
#include <QEvent>

#include <KComboBox>

ModelComboBox::ModelComboBox(QGraphicsWidget *parent)
: Plasma::ComboBox(parent)
, m_isPopupVisible(false)
{
    nativeWidget()->view()->installEventFilter(this);
}

void ModelComboBox::setModel(QObject *model)
{

    qDebug() << "hereADSF *** * " << model;
    QAbstractItemModel *realModel = qobject_cast<QAbstractItemModel*>(model);
    if (realModel){
            nativeWidget()->setModel(realModel);
    }
}

QObject* ModelComboBox::model() const
{
    return nativeWidget()->model();
}

QVariant ModelComboBox::itemData(int index, int role) const
{
    return nativeWidget()->itemData(index, role);
}

bool ModelComboBox::isPopupVisible() const
{
    return m_isPopupVisible;
}

bool ModelComboBox::eventFilter(QObject*, QEvent* event)
{
    switch (event->type()) {
    case QEvent::Show:
        updatePopupVisible(true);
        break;
    case QEvent::Hide:
        updatePopupVisible(false);
        break;
    default:
        break;
    }
    return false;
}

void ModelComboBox::updatePopupVisible(bool value)
{
    if (m_isPopupVisible != value) {
        m_isPopupVisible = value;
        popupVisibleChanged(m_isPopupVisible);
    }
}

int ModelComboBox::indexForData(const QVariant& data, int role) const
{
    QAbstractItemModel* model = nativeWidget()->model();
    for (int row = model->rowCount() - 1; row >= 0; --row) {
        QModelIndex index = model->index(row, 0);
        if (model->data(index, role) == data) {
            return row;
        }
    }
    return -1;
}
