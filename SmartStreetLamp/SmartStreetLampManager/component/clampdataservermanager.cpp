#include "clampdataservermanager.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include "setting/csystemsettingdlg.h"
#include <QList>
#include <QDebug>
CLampDataServerManager* CLampDataServerManager::m_LampDataServer = NULL;
CLampDataServerManager::CLampDataServerManager(QObject* parent)
    :QObject(parent)
{
    gSysSetting->getLampBasicServerInfo(m_dataServerIP, m_dataServerPort);

    m_networkManager = new QNetworkAccessManager(this);
    qDebug()<<"schemes:"<<m_networkManager->supportedSchemes();

    m_networkRequest = new QNetworkRequest();
    m_networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

}

CLampDataServerManager::~CLampDataServerManager()
{

}

CLampDataServerManager* CLampDataServerManager::getInstance()
{
    if(m_LampDataServer == NULL)
    {
        m_LampDataServer = new CLampDataServerManager();
    }

    return m_LampDataServer;
}

QVector<lampInfoStru> CLampDataServerManager::getLampInfoByTableName(QString roadTableName)
{
    QString strUrl = QString("http://%1:%2/operate").arg(m_dataServerIP).arg(m_dataServerPort);
    m_networkRequest->setUrl(QUrl(strUrl));
    //发送post请求获取路灯数据
    QString postData = QString("methodinfo=get-table&db_label=%1").arg(roadTableName);
    qDebug()<<"strUrl:"<<strUrl;
    qDebug()<<"postData:"<<postData;
    QNetworkReply* reply = m_networkManager->post(*m_networkRequest, postData.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QVector<lampInfoStru>  m_lampInfoVec;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyData = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            QVariantList list = document.toVariant().toList();
            QVariantList::iterator  iter = list.begin();
            while(iter != list.end())
            {
                QVariantMap map = (*iter).toMap();
                lampInfoStru  tempLamp;
                tempLamp.lampSerialNumber = map["lampId"].toString();
                tempLamp.belongCityName = map["city"].toString();
                tempLamp.belongRegionName = map["region"].toString();
                tempLamp.belongStreetName = map["street"].toString();
                tempLamp.lampName = tempLamp.belongStreetName + "_" + tempLamp.lampSerialNumber.right(4);
                tempLamp.longitude = map["longitude"].toFloat();
                tempLamp.latitude = map["latitude"].toFloat();
                tempLamp.lampStatus = map["status"].toInt();
                tempLamp.belongCircle = map["circle"].toString();
                tempLamp.controlWay = lampControlWay(map["controlWay"].toInt());
                tempLamp.switchBox = map["switchBox"].toString();
                tempLamp.lampType = map["lampType"].toString();
                tempLamp.lampCount = map["lampCount"].toInt();
                tempLamp.lampHeight = map["height"].toInt();
                tempLamp.department = map["buildDepartment"].toString();
                tempLamp.manufacturer = map["manufacturer"].toString();
                QString strTime = map["buildTime"].toString();
                tempLamp.buildTime = QDate::fromString(strTime, "yyyy-MM-dd");

                m_lampInfoVec.push_back(tempLamp);
                ++iter;
            }
        }
    }
    return m_lampInfoVec;
}

QVector<lampInfoStru> CLampDataServerManager::getLampDataInfo()
{
    QString strUrl = QString("http://%1:%2/operate").arg(m_dataServerIP).arg(m_dataServerPort);
    m_networkRequest->setUrl(QUrl(strUrl));
    //发送post请求获取路灯数据
    QString postData = QString("methodinfo=get-all-tables&db_label=""");
    QNetworkReply* reply = m_networkManager->post(*m_networkRequest, postData.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QVector<lampInfoStru>  m_lampInfoVec;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyData = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            QVariantList list = document.toVariant().toList();
            QVariantList::iterator  iter = list.begin();
            while(iter != list.end())
            {
                QVariantMap map = (*iter).toMap();
                lampInfoStru  tempLamp;
                tempLamp.lampSerialNumber = map["lampId"].toString();
                tempLamp.belongCityName = map["city"].toString();
                tempLamp.belongRegionName = map["region"].toString();
                tempLamp.belongStreetName = map["street"].toString();
                tempLamp.lampName = QString::fromWCharArray(L"路灯") + tempLamp.lampSerialNumber.right(4);
                tempLamp.longitude = map["longitude"].toFloat();
                tempLamp.latitude = map["latitude"].toFloat();
                tempLamp.lampStatus = map["status"].toInt();
                tempLamp.belongCircle = map["circle"].toString();
                tempLamp.controlWay = lampControlWay(map["controlWay"].toInt());
                tempLamp.switchBox = map["switchBox"].toString();
                tempLamp.lampType = map["lampType"].toString();
                tempLamp.lampCount = map["lampCount"].toInt();
                tempLamp.lampHeight = map["height"].toInt();
                tempLamp.department = map["buildDepartment"].toString();
                tempLamp.manufacturer = map["manufacturer"].toString();
                QString strTime = map["buildTime"].toString();
                tempLamp.buildTime = QDate::fromString(strTime, "yyyy-MM-dd");

                m_lampInfoVec.push_back(tempLamp);
                ++iter;
            }
        }
    }
    return m_lampInfoVec;
}



//判断输入的用户名和密码是否正确,正确的话返回userID
bool CLampDataServerManager::checkInputUsername(QString user, QString passwd, int& userID)
{
    //发送本地用户名和密码到服务器端，服务验证
    QString strUrl = QString("http://%1:%2/validate").arg(m_dataServerIP).arg(m_dataServerPort);
    m_networkRequest->setUrl(QUrl(strUrl));
    QString postData = QString("methodinfo=validate-user&username=%1&password=%2").arg(user).arg(passwd);
    QNetworkReply* reply = m_networkManager->post(*m_networkRequest, postData.toUtf8());

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyData = reply->readAll();
        qDebug()<<"replyData:"<<replyData;
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            QVariantMap map = document.toVariant().toMap();
            bool checkFlag = map["status"].toBool();
            if(checkFlag)
            {
                userID = map["userId"].toInt();
                return true;
            }
        }
    }

    return false;
}


