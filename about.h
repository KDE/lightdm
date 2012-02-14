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
#ifndef ABOUT_H
#define ABOUT_H

// Helper function to factorize common part s of KAboutData definition between
// greeter and kcm
static void initAboutData(KAboutData* aboutData)
{
    aboutData->setVersion("0.1");

    aboutData->setShortDescription(ki18n("Login screen using the LightDM framework"));
    aboutData->setLicense(KAboutData::License_GPL);
    aboutData->setCopyrightStatement(ki18n("(c) 2012 David Edmundson"));
    aboutData->setHomepage("https://projects.kde.org/projects/playground/base/lightdm");

    aboutData->addAuthor(ki18n("David Edmundson"), ki18n("Author"), "kde@davidedmundson.co.uk");
    aboutData->addAuthor(ki18n("Aurélien Gâteau"), ki18n("Developer"), "aurelien.gateau@canonical.com");
}

#endif /* ABOUT_H */
