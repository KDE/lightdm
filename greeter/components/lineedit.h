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
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <Plasma/LineEdit>

/** A slightly hacky class to get a version of plasma line edit which can be put in password mode*/
class LineEdit : public Plasma::LineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool passwordMode READ passwordMode WRITE setPasswordMode)

public:
    explicit LineEdit(QGraphicsWidget *parent = 0);

    bool passwordMode() const;
    void setPasswordMode(bool passwordMode);
    
signals:

public slots:
    void setFocus();
};


#endif // PASSWORDLINEEDIT_H
