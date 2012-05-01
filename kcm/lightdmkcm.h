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
#ifndef LIGHTDMKCM_H
#define LIGHTDMKCM_H

#include <KCModule>

class CoreConfig;
class ThemeConfig;

class LightDMKcm : public KCModule
{
    Q_OBJECT
public:
    explicit LightDMKcm(QWidget *parent, const QVariantList &args);

signals:

public slots:
    void save();
    void defaults();

private:
    CoreConfig *m_coreConfig;
    ThemeConfig *m_themeConfig;
};

#endif // LIGHTDMKCM_H
