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
