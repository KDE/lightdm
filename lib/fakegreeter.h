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

#ifndef FAKEGREETER_H
#define FAKEGREETER_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <QLightDM/Greeter>

//this class wraps the real greeter, but doesn't ever actually connect to the backend
//so all method calls fail, we then override some here

class FakeConfig : public QObject
{
    Q_OBJECT
public:
    FakeConfig(QObject *parent);
    Q_INVOKABLE QVariant readEntry(const QString &key) const;
};


class FakeGreeter : public QLightDM::Greeter
{
    Q_OBJECT

    Q_PROPERTY(QString lastLoggedInUser READ lastLoggedInUser CONSTANT)
    Q_PROPERTY(QString guestLoginName READ guestLoginName CONSTANT)

public:
    explicit FakeGreeter(QObject* parent=0);
    virtual ~FakeGreeter();
    QString lastLoggedInUser() const;
    QString guestLoginName() const;

    bool isAuthenticated() const;


public slots:
    bool connectSync();
    void authenticate(const QString &username=QString());
    void authenticateAsGuest();
    void respond(const QString &response);
//     void cancelAuthentication();
//     void setLanguage (const QString &language);
    bool startSessionSync(const QString &session=QString());

private:
    bool m_isAuthenticated;
};

#endif // FAKEGREETER_H
