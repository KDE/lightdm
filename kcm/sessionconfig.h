#ifndef SESSIONCONFIG_H
#define SESSIONCONFIG_H

#include <QWidget>

namespace Ui {
    class SessionConfig;
}

class SessionConfig : public QWidget
{
    Q_OBJECT

public:
    explicit SessionConfig(QWidget *parent = 0);
    ~SessionConfig();

private:
    Ui::SessionConfig *ui;
};

#endif // SESSIONCONFIG_H
