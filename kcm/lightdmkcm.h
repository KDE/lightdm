#ifndef LIGHTDMKCM_H
#define LIGHTDMKCM_H

#include <KCModule>

class CoreConfig;
class ThemeConfig;

class LightDMKcm : public KCModule
{
    Q_OBJECT
public:
    explicit LightDMKcm(QWidget *parent, const QVariantList &args);

signals:

public slots:
    void save();

private:
    CoreConfig *m_coreConfig;
    ThemeConfig *m_themeConfig;
};

#endif // LIGHTDMKCM_H
