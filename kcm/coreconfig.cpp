#include "coreconfig.h"
#include "ui_coreconfig.h"

CoreConfig::CoreConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoreConfig)
{
    ui->setupUi(this);
}

CoreConfig::~CoreConfig()
{
    delete ui;
}
