#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <QWidget>
#include <QDir>

#include <KSharedConfigPtr>
#include <KSharedConfig>

class AuthKitConfigLoader;
class KConfigDialogManager;

/** This class shows all config options for a given theme, and updates a shared KConfig*/

class ConfigOptions : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigOptions(QWidget *parent = 0);

    void setConfig(const KSharedConfigPtr &config);
    void setTheme(const QDir &themeDir);

    QVariantMap save();
    void defaults();

signals:
    void changed(bool);

public slots:

private slots:
    void onSettingsChanged();

private:
    KSharedConfigPtr m_config;

    QWeakPointer<QWidget> m_wrapperWidget;
    KConfigDialogManager* m_manager;
    AuthKitConfigLoader* m_configLoader;
};

#endif // CONFIGOPTIONS_H
