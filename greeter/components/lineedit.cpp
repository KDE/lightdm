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
#include "lineedit.h"

#include <KLineEdit>

LineEdit::LineEdit(QGraphicsWidget *parent) :
    Plasma::LineEdit(parent)
{
}

bool LineEdit::passwordMode() const
{
    return nativeWidget()->echoMode() == QLineEdit::Password;
}

void LineEdit::setPasswordMode(bool passwordMode)
{
    if (passwordMode) {
        nativeWidget()->setEchoMode(QLineEdit::Password);
    } else {
        nativeWidget()->setEchoMode(QLineEdit::Normal);
    }
}

void LineEdit::setFocus()
{
    nativeWidget()->setFocus();
}
