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
#include "themeconfig.h"
#include "ui_themeconfig.h"

#include "themesmodel.h"

#include <QUiLoader>
#include <QFile>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDir>

#include <KMessageBox> //note only used for temporary warning.
#include <KDebug>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KConfigDialog>
#include <KConfigSkeletonItem>


#include <KAuth/Action>
#include <KAuth/ActionReply>

#include <Plasma/ConfigLoader>

#include "config.h"

class AuthKitConfigLoader : public Plasma::ConfigLoader {
public:
    AuthKitConfigLoader(KSharedConfigPtr config, QIODevice *xml, QObject *parent=0);
protected:
    void usrWriteConfig();
};

AuthKitConfigLoader::AuthKitConfigLoader(KSharedConfigPtr config, QIODevice *xml, QObject *parent)
    : Plasma::ConfigLoader(config, xml, parent)
{}

//using the normal KConfigSkeleton we can't write to the config, so we need to use authkit.
void AuthKitConfigLoader::usrWriteConfig() {
    kDebug() << "user write config";

    KAuth::Action saveAction("org.kde.kcontrol.kcmlightdm.savethemedetails");
    saveAction.setHelperID("org.kde.kcontrol.kcmlightdm");

    QVariantMap args;
    foreach(KConfigSkeletonItem* item, items()) {
        args[item->key()] = item->property();
    }

    saveAction.setArguments(args);

    KAuth::ActionReply reply = saveAction.execute();
    if (reply.failed()) {
        qDebug() << reply.errorCode();
        qDebug() << KAuth::ActionReply::NoSuchAction;
        qDebug() << reply.errorDescription();
        qDebug() << "save failed :-(";
    } else {
        qDebug() << "save ok!";
    }
}

ThemeConfig::ThemeConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemeConfig)
{
    m_config = KSharedConfig::openConfig(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");

    ui->setupUi(this);

    ThemesModel *model = new ThemesModel(this);
    ui->themesList->setModel(model);

    connect(ui->themesList, SIGNAL(activated(QModelIndex)), SLOT(onThemeSelected(QModelIndex)));
    connect(ui->themesList, SIGNAL(clicked(QModelIndex)), SLOT(onThemeSelected(QModelIndex)));
    connect(ui->configureButton, SIGNAL(released()), SLOT(onConfigureClicked()));

    QString theme = m_config->group("greeter").readEntry("theme-name", "shinydemo");

    //set the UI to show the correct item if available.
    for (int i=0;i < model->rowCount(QModelIndex()); i++) {
        QModelIndex index = model->index(i);
        if (index.data(ThemesModel::IdRole).toString() == theme) {
            ui->themesList->setCurrentIndex(index);
            onThemeSelected(index);
        }
    }

}

ThemeConfig::~ThemeConfig()
{
    delete ui;
}

void ThemeConfig::onThemeSelected(const QModelIndex &index)
{
    ui->nameLabel->setText(index.data().toString());
    ui->descriptionLabel->setText(index.data(ThemesModel::DescriptionRole).toString());
    ui->authorLabel->setText(index.data(ThemesModel::AuthorRole).toString());

    //FUTURE, simply load the actual QML, specify an arbitrary screen size and scale the contents.
    //we can add fake components for the greeter so QML thinks it's working.

    //can't do this easily now as we need our private kdeclarative library and my widgets from the greeter
    //could make a private lib for all this - but that seems overkill when we won't need any of that in the 4.8 only versions.
 
    QPixmap preview = index.data(ThemesModel::PreviewRole).value<QPixmap>();
    if (! preview.isNull()) {
        ui->preview->setPixmap(preview.scaled(QSize(250,250), Qt::KeepAspectRatio));
    } else
    {
        ui->preview->setPixmap(QPixmap());
    }

    QDir dir = themeDir();
    ui->configureButton->setVisible(
        dir.exists(QLatin1String("main.xml"))
        && dir.exists(QLatin1String("config.ui"))
        );

    emit changed(true);
}

QDir ThemeConfig::themeDir() const
{
    return QDir(ui->themesList->currentIndex().data(ThemesModel::PathRole).toString());
}

void ThemeConfig::onConfigureClicked()
{
    kDebug();

    //FIXME I'd like to replace to have the widgets inline rather than in a dialog.

    QDir dir = themeDir();

    QFile kcfgFile(dir.filePath(QLatin1String("main.xml")));
    kcfgFile.open(QFile::ReadOnly);
    AuthKitConfigLoader configLoader(m_config, &kcfgFile, this);

    QUiLoader loader;
    QFile uiFile(dir.filePath(QLatin1String("config.ui")));
    QWidget* widget = loader.load(&uiFile, this);

    KConfigDialog dialog(this, QLatin1String("theme-config"), &configLoader);
    dialog.setFaceType(KPageDialog::Plain);
    dialog.addPage(widget, i18n("Configure Theme"));
    dialog.exec();
}

void ThemeConfig::save()
{
    QModelIndex currentIndex = ui->themesList->currentIndex();
    if (currentIndex.isValid()) {
        KAuth::Action saveAction("org.kde.kcontrol.kcmlightdm.savetheme");
        saveAction.setHelperID("org.kde.kcontrol.kcmlightdm");
        QVariantMap args;
        args["theme-name"] = currentIndex.data(ThemesModel::IdRole);
        saveAction.setArguments(args);

        KAuth::ActionReply reply = saveAction.execute();
        if (reply.failed()) {
            qDebug() << reply.errorCode();
            qDebug() << KAuth::ActionReply::NoSuchAction;
            qDebug() << reply.errorDescription();
            qDebug() << "save failed :-(";
        } else {
            qDebug() << "save ok!";
        }
    }

}

#include "moc_themeconfig.cpp"
