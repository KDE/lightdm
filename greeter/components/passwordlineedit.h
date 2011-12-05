#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <Plasma/LineEdit>

/** A slightly hacky class to get a version of plasma line edit which can be put in password mode*/
class PasswordLineEdit : public Plasma::LineEdit
{
    Q_OBJECT
public:
    explicit PasswordLineEdit(QGraphicsWidget *parent = 0);

signals:

public slots:
    void setFocus();
};


#endif // PASSWORDLINEEDIT_H
