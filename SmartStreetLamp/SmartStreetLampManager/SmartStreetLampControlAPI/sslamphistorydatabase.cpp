#include "sslamphistorydatabase.h"

CSSLampHistorydataBase::CSSLampHistorydataBase()
{
    mStrHost = "192.168.200.36";
    mStrPort = "5000";
}
void CSSLampHistorydataBase::setHost(const QString& strHost)
{
    mStrHost = strHost;
}
void CSSLampHistorydataBase::setPort(const QString& strPort)
{
    mStrPort = strPort;
}
