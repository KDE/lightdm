#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "themesmodel.h"

#include <QUiLoader>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ThemesModel *model = new ThemesModel(this);
    ui->themesList->setModel(model);
    connect(ui->themesList, SIGNAL(activated(QModelIndex)), SLOT(onThemeSelected(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onThemeSelected(const QModelIndex &index)
{
    ui->nameLabel->setText(index.data().toString());
    ui->descriptionLabel->setText(index.data(Qt::ToolTipRole).toString());
    ui->previewLabel->setPixmap(index.data(ThemesModel::PreviewRole).value<QPixmap>());


    QUiLoader *loader = new QUiLoader;
    QFile uiFile("/home/david/lightdm/lightdm_kcm/sessionconfig.ui");
    QWidget* widget = loader->load(&uiFile, this);
    this->layout()->addWidget(widget);
}
