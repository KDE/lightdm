#ifndef MODELCOMBOBOX_H
#define MODELCOMBOBOX_H

#include <Plasma/ComboBox>

class QAbstractItemModel;

class ModelComboBox : public Plasma::ComboBox
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model WRITE setModel)

    /**
     * popupVisible is true when the user clicks on the comboxbox to show the
     * list, false otherwise.
     */
    Q_PROPERTY(bool popupVisible READ isPopupVisible NOTIFY popupVisibleChanged)

public:
    explicit ModelComboBox(QGraphicsWidget *parent = 0);

    //I really want this to be QAbstractItemModel* but QML fails if I do, probably a registering type issue. ideas?
    void setModel(QObject *model);
    QObject* model() const;

    Q_INVOKABLE QVariant itemData(int index, int role = Qt::UserRole) const;

    Q_INVOKABLE bool isPopupVisible() const;

    Q_INVOKABLE int indexForData(const QVariant& data, int role = Qt::UserRole) const;

Q_SIGNALS:
    void popupVisibleChanged(bool);

protected:
    bool eventFilter(QObject*, QEvent* event);

private:
    bool m_isPopupVisible;

    void updatePopupVisible(bool);
};

#endif // MODELCOMBOBOX_H
