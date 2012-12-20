#ifndef GREETERPREVIEW_H
#define GREETERPREVIEW_H

#include <QDeclarativeView>
#include <KConfig>
#include <KSharedConfigPtr>

class GreeterPreview : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit GreeterPreview(QWidget *parent = 0);
    void setTheme(const QString &theme);
    void setConfig(const KSharedConfigPtr &config);
signals:
    
public slots:
    void reload();
    
private:
    QString m_theme;
    KSharedConfigPtr m_config;
};

#endif // GREETERPREVIEW_H
