#include "sslamphistorydata.h"
#include <QEventLoop>
#include <QJsonDocument>
CSSLampHistorydata::CSSLampHistorydata()
{
}
QVariantList CSSLampHistorydata::getHistoryData(qint64 beginTimestamp, qint64 endTimestamp
                                                , const QString& lampID, const QString& valueType)
{
    qDebug()<<beginTimestamp<<"-------------"<<endTimestamp;
    QString strUrl = QString("http://%1:%2/v1/GetHistoryData?beginTimestamp=%3&endTimestamp=%4&lampID=%5&valueType=%6")
            .arg(mStrHost).arg(mStrPort).arg(beginTimestamp).arg(endTimestamp).arg(lampID).arg(valueType);
    qDebug()<<"getHistoryData : "<<strUrl;
    QUrl url = QUrl(strUrl);
    setAccept("*/*");
    mHeaderMap.clear();
    mHeaderMap["Content-Type"] = "application/json";
    mHeaderMap["Host"] = QString("%1:%2").arg(mStrHost).arg(mStrPort);
    mHeaderMap["accept-encoding"] = "gzip, deflate";

    QNetworkReply *reply = get(url);
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if(QNetworkReply::NoError == reply->error() )
    {
        QByteArray data = reply->readAll();
        qDebug()<<data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isObject())
        {
            qDebug()<<"return 1";
            QVariantMap map = jsonDoc.toVariant().toMap();
            if(map.find("evetn") != map.end())
            {
                qDebug()<<"return 2";
                QVariant var = map["evetn"];
                if(var.canConvert(QVariant::List))
                {
                    qDebug()<<"return !!!!";
                    //qDebug()<<var;
                    return var.toList();
                }
            }
        }

    }else{
        qDebug()<<"controlByConfig"<< reply->errorString();
    }
    return QVariantList();
}




QNetworkRequest CSSLampHistorydata::createRequest(const QUrl &url)const
{
    QNetworkRequest request = QNetworkRequest(url);
    QList<QString> keys = mHeaderMap.keys();
    for(auto iter = keys.begin(); iter != keys.end(); ++iter)
    {
        request.setRawHeader( (*iter).toUtf8(), (mHeaderMap[*iter]).toString().toUtf8());
    }
    return request;
}













