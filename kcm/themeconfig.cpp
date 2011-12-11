#include "themeconfig.h"
#include "ui_themeconfig.h"

#include "themesmodel.h"

#include <QUiLoader>
#include <QFile>
#include <QHBoxLayout>
#include <QPixmap>

#include <KMessageBox> //note only used for temporary warning.
#include <KDebug>

#include <KConfig>
#include <KSharedConfigPtr>

#include "config.h"


ThemeConfig::ThemeConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemeConfig)
{
    m_config = KSharedConfig::openConfig(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");

    //temporarily warn user if config file is read only.
    if (m_config->accessMode() != KConfig::ReadWrite) {
        KMessageBox::error(this,
                           QString("Write access is needed to %1 for saving. This is only temporary because I haven't implemented PolKit stuff yet. Saving will probably not work.").arg(QString(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf")));
    }

    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    ui->optionsWidget->setLayout(layout);

    ThemesModel *model = new ThemesModel(this);
    ui->themesList->setModel(model);

    connect(ui->themesList, SIGNAL(activated(QModelIndex)), SLOT(onThemeSelected(QModelIndex)));


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


//    //this is just test code for loading config options.
//    QUiLoader loader;
//    QFile uiFile("/home/david/lightdm/lightdm-kde/kcm/sessionconfig.ui");
//    QWidget* widget = loader.load(&uiFile, this);

//    if(ui->optionsWidget->layout()->count() > 0) {
//        delete ui->optionsWidget->layout()->itemAt(0);
//    }

    //    ui->optionsWidget->layout()->addWidget(widget);

    emit changed(true);
}

void ThemeConfig::save()
{

    qDebug() << "saving";
    //save to a config
    QModelIndex currentIndex = ui->themesList->currentIndex();
    if (currentIndex.isValid()) {
        QString themeName = currentIndex.data(ThemesModel::IdRole).toString();
        m_config->group("greeter").writeEntry("theme-name", themeName);
    }
    m_config->sync();
}

#include "moc_themeconfig.cpp"
