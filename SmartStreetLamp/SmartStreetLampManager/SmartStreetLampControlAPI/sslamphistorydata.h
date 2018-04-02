#ifndef SSLAMPHISTORYDATA_H
#define SSLAMPHISTORYDATA_H
#include "apibase.h"
#include "sslamphistorydatabase.h"
#include <QDateTime>
class CSSLampHistorydata : public APIBase,
                      public CSSLampHistorydataBase
{
    Q_OBJECT
public:
    CSSLampHistorydata();
public:
    QVariantList getHistoryData(qint64 beginTimestamp, qint64 endTimestamp
                                , const QString& lampID, const QString& valueType);
public:
    virtual QNetworkRequest createRequest(const QUrl &url) const;
private:
    QVariantMap mHeaderMap;
};

#endif // SSLAMPHISTORYDATA_H
