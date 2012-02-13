#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QWidget>

#include <KSharedConfig>

namespace Ui {
    class ThemeConfig;
}

class QDir;
class QModelIndex;

class ThemeConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeConfig(QWidget *parent = 0);
    ~ThemeConfig();

    QVariantMap save();

signals:
    void changed(bool);

private slots:
    void onThemeSelected(const QModelIndex &index);
    void onConfigureClicked();

private:
    KSharedConfigPtr m_config;
    Ui::ThemeConfig *ui;

    QDir themeDir() const;

};

#endif // THEMECONFIG_H
