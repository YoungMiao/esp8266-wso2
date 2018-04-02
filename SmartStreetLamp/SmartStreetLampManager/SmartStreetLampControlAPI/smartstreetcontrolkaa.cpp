#include "smartstreetcontrolkaa.h"
#include <QEventLoop>
#include <QFile>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>

CSmartStreetControlKaa::CSmartStreetControlKaa(QObject *parent) : APIBase(parent)
{
    mStrConfigPath = "D:\\notificationConfig\\";

    //获取配置文件数据
    QString path = QCoreApplication::applicationDirPath();
    QFile file(path + "\\conf\\restInfo.json");
    qDebug()<<path;
    if(file.open(QFile::ReadOnly))
    {
        QByteArray jsondata = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsondata);
        if(jsonDoc.isObject())
        {
            QVariant var = jsonDoc.toVariant();
            QVariantMap map = var.toMap();
            strKaaControlIp = map["KaaControlIp"].toString();
            strKaaControlPort = map["KaaControlPort"].toString();
            strKaaUser = map["KaaUser"].toString();
            strKaaPasswd = map["KaaPasswd"].toString();
            strApplicationID = map["ApplicationID"].toString();
            groupIDList = map["groupIDs"].toList();
            //qDebug()<<"groupIDList : "<<groupIDList.size();
            strConfigSchemaId = map["ConfigSchemaId"].toString();
            return ;
        }
    }
    qDebug()<<"rest control config error !!!!";
    exit(-1);
    return ;
}

bool CSmartStreetControlKaa::TurnOnLampByLampid(const QString& lampId)
{
    qDebug()<<"error:1111111111";
    return true;
}


bool CSmartStreetControlKaa::TurnOnLampByStrategy(const QString& streetID,
                                  const QString& strategyName)
{
    QString groupID;
    for(auto it : groupIDList)
    {
        QVariantMap map = it.toMap();
        if(map["StreetID"].toString() == streetID)
        {
            groupID = map["Id"].toString();
        }
    }
    controlByConfig(groupID, strategyName);
    return true;
}
bool CSmartStreetControlKaa::TurnOffLampByStrategy(const QString& streetID,
                                   const QString& strategyName)
{
    QString groupID;
    for(auto it : groupIDList)
    {
        QVariantMap map = it.toMap();
        if(map["StreetID"].toString() == streetID)
        {
            groupID = map["Id"].toString();
        }
    }
    controlByConfig(groupID, strategyName);
    return true;
}
void CSmartStreetControlKaa::controlByFile(const QString& strNotificationPath,
                           const QString& strFilePath, const QString& strLampID)
{
     QUrl url = QUrl("http://192.168.200.36:8080/kaaAdmin/rest/api/sendNotification");
     if(strLampID.isEmpty())
     {
         url = QUrl("http://192.168.200.36:8080/kaaAdmin/rest/api/sendNotification");
     }else{
         url = QUrl("http://192.168.200.36:8080/kaaAdmin/rest/api/sendUnicastNotification");
     }
     QString strAuth = "Basic ";
     QString strUserPwd = "devuser";
     strUserPwd += ":";
     strUserPwd += "devuser123";
     QByteArray arrBasic = strUserPwd.toUtf8();
     QString strBasic = arrBasic.toBase64();
     strAuth += strBasic;
     qDebug()<<strAuth;
     setAuthToken(strAuth.toUtf8());

     QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

     QHttpPart notificationPart;
     notificationPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
     notificationPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                QVariant("form-data; name=\"notification\"; filename=\"notification.json\""));/* version.tkt is the name on my Disk of the file that I want to upload */
     QHttpPart filePart;
     filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
     filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"file.json\""));
     //textPart.setBody("toto");/* toto is the name I give to my file in the server */

     //QHttpPart EndpointIdPart;
     //EndpointIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"file.json\""));
     QFile *notificationFile = new QFile(strNotificationPath);
     notificationFile->open(QIODevice::ReadOnly);
     notificationPart.setBodyDevice(notificationFile);
     notificationFile->setParent(multiPart);
     QFile *file = new QFile(strFilePath);
     file->open(QIODevice::ReadOnly);
     filePart.setBodyDevice(file);
     file->setParent(multiPart);


     multiPart->append(notificationPart);
     multiPart->append(filePart);


     QNetworkReply *reply = post(url, multiPart);
     QEventLoop eventLoop;
     connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
     eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

     qDebug()<<reply->errorString();
     qDebug()<<reply->readAll();
     return ;
}

void CSmartStreetControlKaa::controlByConfig(const QString& strGroupID, const QString& strStrategy)
{
    if(strGroupID.isEmpty())
    {
        qDebug()<<"streetID is not config!!!";
        return ;
    }
    int samplingPeriod = 10;
    QString strUrl = QString("http://%1:%2/kaaAdmin/rest/api/configuration").arg(strKaaControlIp).arg(strKaaControlPort);
    qDebug()<<"strUrl:"<<strUrl;
    QUrl url = QUrl(strUrl);
    QString strAuth = "Basic ";
    QString strUserPwd = strKaaUser;
    strUserPwd += ":";
    strUserPwd += strKaaPasswd;
    qDebug()<<strUrl;
    qDebug()<<strUserPwd;
    QByteArray arrBasic = strUserPwd.toUtf8();
    QString strBasic = arrBasic.toBase64();
    strAuth += strBasic;
    qDebug()<<strAuth;
    setAuthToken(strAuth.toUtf8());


    QByteArray byteStrategy = strStrategy.toUtf8();
    QString strStrategyBase = byteStrategy.toBase64();
    mHeaderMap.clear();
    mHeaderMap["Content-Type"] = "application/json";
    QString strConfig = QString("{\\\"samplingPeriod\\\":{\\\"int\\\":%1},\\\"LampStrategy\\\":{\\\"string\\\":\\\"%2\\\"}}").arg(samplingPeriod).arg(strStrategyBase);
    QString strJson = QString("{\"applicationId\": \"%1\",\"endpointGroupId\": \"%2\",\
        \"body\": \"%3\",\
        \"schemaId\": \"%4\"}").arg(strApplicationID).arg(strGroupID)
    .arg(strConfig).arg(strConfigSchemaId);
    //mHeaderMap["Content-Length"] = strJson.toUtf8().length();


    QNetworkReply *reply = post(url, strJson.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if(QNetworkReply::NoError == reply->error() )
    {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(!jsonDoc.isObject())
        {
            qDebug()<<"format error!";
            return ;
        }
        QVariantMap map = jsonDoc.toVariant().toMap();
        ActiveConfigById(map["id"].toString());
    }else{
        qDebug()<<"controlByConfig"<< reply->errorString();
    }
    return ;
}
void CSmartStreetControlKaa::ActiveConfigById(const QString& configID)
{
    QString strUrl = QString("http://%1:%2/kaaAdmin/rest/api/activateConfiguration").arg(strKaaControlIp).arg(strKaaControlPort);
    QUrl url = QUrl(strUrl);
    //QUrl url = QUrl("http://192.168.200.36:8080/kaaAdmin/rest/api/activateConfiguration");
    QString strAuth = "Basic ";
    QString strUserPwd = strKaaUser;
    strUserPwd += ":";
    strUserPwd += strKaaPasswd;
    QByteArray arrBasic = strUserPwd.toUtf8();
    QString strBasic = arrBasic.toBase64();
    strAuth += strBasic;
    qDebug()<<strAuth;
    setAuthToken(strAuth.toUtf8());

    mHeaderMap.clear();
    mHeaderMap["Content-Type"] = "application/json";
    QNetworkReply *reply = post(url, configID.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(QNetworkReply::NoError == reply->error() )
    {
        //qDebug()<<reply->readAll();
        qDebug()<<"ActiveConfigById OK";
    }else{
        qDebug()<<"ActiveConfigById"<< reply->errorString();
    }
    return ;
}















QNetworkRequest CSmartStreetControlKaa::createRequest(const QUrl &url)const
{
    QNetworkRequest request = QNetworkRequest(url);
    QList<QString> keys = mHeaderMap.keys();
    for(auto iter = keys.begin(); iter != keys.end(); ++iter)
    {
        request.setRawHeader( (*iter).toUtf8(), (mHeaderMap[*iter]).toString().toUtf8());
    }
    return request;
}
