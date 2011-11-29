#include "sessionconfig.h"
#include "ui_sessionconfig.h"

SessionConfig::SessionConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionConfig)
{
    ui->setupUi(this);
}

SessionConfig::~SessionConfig()
{
    delete ui;
}
