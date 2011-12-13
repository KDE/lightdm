#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H

#include <QObject>
#include <KSharedConfig>

/** This class exposes the lightdm-kde config to QML*/

class ConfigWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWrapper(QObject *parent = 0);

    Q_INVOKABLE QVariant readEntry(const QString &key, const QVariant &aDefault=QVariant()) const;

private:
    KSharedConfigPtr m_config;

};

#endif // CONFIGWRAPPER_H
