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
#ifndef GREETER_WINDOW_H
#define GREETER_WINDOW_H

#include <QDeclarativeView>

namespace QLightDM {
    class Greeter;
}

class GreeterWindow: public QDeclarativeView
{
    Q_OBJECT

public:
    GreeterWindow(QWidget *parent = 0);

    ~GreeterWindow();

protected:
    void resizeEvent(QResizeEvent *);

private Q_SLOTS:
    void screenshot();

private:
    QLightDM::Greeter *m_greeter;

};

#endif  //  GREETER_WINDOW_H

