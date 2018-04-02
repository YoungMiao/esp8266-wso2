#ifndef SSLAMPHISTORYDATABASE_H
#define SSLAMPHISTORYDATABASE_H
#include <QVariantMap>

class CSSLampHistorydataBase
{
public:
    CSSLampHistorydataBase();
public:
    virtual QVariantList getHistoryData(qint64 beginTimestamp, qint64 endTimestamp
                                        , const QString& lampID, const QString& valueType) = 0;


    void setHost(const QString& strHost);
    void setPort(const QString& strPort);
protected:
    QString mStrHost;
    QString mStrPort;
};

#endif // SSLAMPHISTORYDATABASE_H
