#ifndef SESSIONSMODEL_H
#define SESSIONSMODEL_H

#include "extrarowproxymodel.h"

class SessionsModel : public ExtraRowProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool showLastUsedSession READ showLastUsedSession WRITE setShowLastUsedSession)
public:
    explicit SessionsModel(QObject *parent = 0);
    
    /** Add a row to the sessions model titled "Last Used Session" */
    void setShowLastUsedSession(bool showLastUsedSession);
    bool showLastUsedSession() const;

signals:
    
public slots:

private:
    bool m_showLastUsedSession;
    
};

#endif // SESSIONSMODEL_H
