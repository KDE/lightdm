#ifndef GREETER_WINDOW_H
#define GREETER_WINDOW_H

#include <QDeclarativeView>

namespace QLightDM {
    class Greeter;
}

class GreeterWindow: public QDeclarativeView
{
    Q_OBJECT

public:
    GreeterWindow(QWidget *parent = 0);

    ~GreeterWindow();

protected:
    void resizeEvent(QResizeEvent *);

private:
    QLightDM::Greeter *m_greeter;

};

#endif  //  GREETER_WINDOW_H

