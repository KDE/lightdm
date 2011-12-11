#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QWidget>

namespace Ui {
    class ThemeConfig;
}

class QModelIndex;

class ThemeConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeConfig(QWidget *parent = 0);
    ~ThemeConfig();

private slots:
    void onThemeSelected(const QModelIndex &index);
    void onApply();


private:
    Ui::ThemeConfig *ui;
};

#endif // THEMECONFIG_H
