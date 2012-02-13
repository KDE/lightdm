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

QVariantMap ThemeConfig::save()
{
    QModelIndex currentIndex = ui->themesList->currentIndex();
    if (!currentIndex.isValid()) {
        return QVariantMap();
    }
    QVariantMap args;
    args["greeter/greeter/theme-name"] = currentIndex.data(ThemesModel::IdRole);
    return args;
}

#include "moc_themeconfig.cpp"
