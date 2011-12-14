#ifndef HELPER_H
#define HELPER_H

#include <KAuth/ActionReply>
#include <KAuth/HelperSupport>

using namespace KAuth;

class Helper: public QObject
{
    Q_OBJECT
public slots:
    ActionReply savetheme(const QVariantMap &args);
    ActionReply savethemedetails(const QVariantMap &args);
};

#endif
