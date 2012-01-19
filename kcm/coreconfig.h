#ifndef CORECONFIG_H
#define CORECONFIG_H

#include <QWidget>

namespace Ui {
    class CoreConfig;
}

class CoreConfig : public QWidget
{
    Q_OBJECT

public:
    explicit CoreConfig(QWidget *parent = 0);
    ~CoreConfig();

private:
    void loadFromConfig();

    Ui::CoreConfig *ui;
};

#endif // CORECONFIG_H
