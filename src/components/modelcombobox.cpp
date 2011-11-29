#include "modelcombobox.h"

#include <QAbstractItemModel>
#include <QDebug>

#include <KComboBox>

ModelComboBox::ModelComboBox(QGraphicsWidget *parent) :
    Plasma::ComboBox(parent)
{
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


