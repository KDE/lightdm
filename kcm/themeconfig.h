#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QWidget>

#include <KConfigGroup>

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

public slots:
    void save();

private slots:
    void onThemeSelected(const QModelIndex &index);

private:

    KConfigGroup m_config;
    Ui::ThemeConfig *ui;
};

#endif // THEMECONFIG_H
