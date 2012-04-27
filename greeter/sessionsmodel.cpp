#include "sessionsmodel.h"

#include <QLightDM/SessionsModel>

#include <KLocalizedString>

SessionsModel::SessionsModel(QObject *parent) :
    ExtraRowProxyModel(parent),
    m_showLastUsedSession(false)
{
    setSourceModel(new QLightDM::SessionsModel(this));


}

void SessionsModel::setShowLastUsedSession(bool showLastUsedSession)
{
    if (showLastUsedSession == m_showLastUsedSession) {
        return;
    }
    m_showLastUsedSession = showLastUsedSession;


    if (m_showLastUsedSession) {
        int lastSessionId = appendRow();
        setRowText(lastSessionId, 0, i18n("Previously Used Session"));
        //if lightDM is given an empty session parameter it will just guess something sensible. (l
        setRowData(lastSessionId, 0, "", QLightDM::SessionsModel::KeyRole);
        //don't set any data.. no session ID = "", which means LightDM will default to whatever it
    } //else ...
}

bool SessionsModel::showLastUsedSession() const
{
    return m_showLastUsedSession;
}
