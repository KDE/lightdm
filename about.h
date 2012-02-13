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
