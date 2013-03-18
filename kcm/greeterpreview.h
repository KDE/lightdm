#ifndef GREETERPREVIEW_H
#define GREETERPREVIEW_H

#include <QDeclarativeView>
#include <KConfig>
#include <KSharedConfigPtr>

#include <Plasma/Package>

class GreeterPreview : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit GreeterPreview(QWidget *parent = 0);
    void setTheme(const Plasma::Package &theme);
    void setConfig(const KSharedConfigPtr &config);
signals:
    
public slots:
    void reload();
    
private:
    Plasma::Package m_theme;
    KSharedConfigPtr m_config;
};

#endif // GREETERPREVIEW_H
