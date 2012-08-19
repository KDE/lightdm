/*
This file is part of LightDM-KDE.

Copyright 2012 David Edmundson <kde@davidedmundson.co.uk>

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

#ifndef GREETERWRAPPER_H
#define GREETERWRAPPER_H

#include <QLightDM/Greeter>
#include <KSharedConfig>

class GreeterWrapper : public QLightDM::Greeter
{
    Q_OBJECT

    Q_PROPERTY(QString lastLoggedInUser READ lastLoggedInUser CONSTANT)
    Q_PROPERTY(QString guestLoginName READ guestLoginName CONSTANT)

public:
    explicit GreeterWrapper(QObject *parent = 0);

    QString lastLoggedInUser() const;
    QString guestLoginName() const;

signals:
    void aboutToLogin();

public slots:
    bool startSessionSync(const QString &session=QString());

private:
    void saveLastUser(const QString &user);
    KSharedConfig::Ptr m_config;
};

#endif // GREETERWRAPPER_H
