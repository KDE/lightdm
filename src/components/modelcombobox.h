#ifndef MODELCOMBOBOX_H
#define MODELCOMBOBOX_H

#include <Plasma/ComboBox>

class QAbstractItemModel;

class ModelComboBox : public Plasma::ComboBox
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model WRITE setModel)

public:
    explicit ModelComboBox(QGraphicsWidget *parent = 0);

    //I really want this to be QAbstractItemModel* but QML fails if I do, probably a registering type issue. ideas?
    void setModel(QObject *model);
    QObject* model() const;

    Q_INVOKABLE QVariant itemData(int index, int role = Qt::UserRole) const;
};

#endif // MODELCOMBOBOX_H
