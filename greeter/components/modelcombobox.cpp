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
