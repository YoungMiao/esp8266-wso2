#include "csystemsettingdlg.h"
#include "ui_syssetting.h"
#include <QStringListModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QDebug>
CSystemSettingDlg* CSystemSettingDlg::m_sysSettingDlg = NULL;
CSystemSettingDlg::CSystemSettingDlg()
    :ui(new Ui::CSystemSettingDlg)
{
    ui->setupUi(this);
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->applyButton, SIGNAL(pressed()),this, SLOT(saveSettingDatas()));
}

CSystemSettingDlg::~CSystemSettingDlg()
{

}


CSystemSettingDlg* CSystemSettingDlg::getInstance()
{
    if(m_sysSettingDlg == NULL)
    {
        m_sysSettingDlg = new CSystemSettingDlg();
        m_sysSettingDlg->initSystemSetting();

    }
    return m_sysSettingDlg;
}

//初始化界面
void CSystemSettingDlg::initSystemSetting()
{
    QListWidgetItem* itemGeneral = new QListWidgetItem(QString::fromWCharArray(L"通用设置"));
    QListWidgetItem* itemSys = new QListWidgetItem(QString::fromWCharArray(L"系统设置"));
    QListWidgetItem* itemStrategy = new QListWidgetItem(QString::fromWCharArray(L"策略设置"));
    QListWidgetItem* itemPara = new QListWidgetItem(QString::fromWCharArray(L"数据源设置"));
    ui->sysSetting_listWidget->addItem(itemGeneral);
    ui->sysSetting_listWidget->addItem(itemSys);
    ui->sysSetting_listWidget->addItem(itemStrategy);
    ui->sysSetting_listWidget->addItem(itemPara);

    ui->sysSetting_listWidget->setCurrentItem(itemGeneral);
    initDataSourcePagePara();
    connect(ui->sysSetting_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showTabInfo(QListWidgetItem*)));
}

//保存界面设置的值
void CSystemSettingDlg::saveSettingDatas()
{

}

//重置界面值
void CSystemSettingDlg::resetSettingParas()
{

}

void CSystemSettingDlg::showTabInfo(QListWidgetItem* curItem)
{
    int row = ui->sysSetting_listWidget->currentRow();
    ui->stackedWidget->setCurrentIndex(row);

}

void CSystemSettingDlg::initGeneralPagePara()
{

}

void CSystemSettingDlg::initSystemPagePara()
{

}

void CSystemSettingDlg::initDataSourcePagePara()
{

    m_LampBasicServerHost = ui->lineEdit_LampIP->text();
    m_LampBasicServerPort = ui->lineEdit_LampPort->text().toInt();

    m_RealServerHost = ui->lineEdit_RealIP->text();
    m_RealServerPort = ui->lineEdit_RealPort->text().toInt();


    m_HistServerHost = ui->lineEdit_HistIP->text();
    m_HistServerPort = ui->lineEdit_HistPort->text().toInt();

    m_WSO2ServerHost = ui->lineEdit_WSO2Host->text();
    m_WSO2Username = ui->lineEdit_username->text();
    m_WSO2Password = ui->lineEdit_password->text();
    m_WSO2ServerHTTPPort = ui->lineEdit_httpport->text().toInt();
    m_WSO2ServerHTTPSPort = ui->lineEdit_httpsport->text().toInt();
    m_WSO2ServerMQTTPort = ui->lineEdit_MQTTPort->text().toInt();

}

void CSystemSettingDlg::getLampBasicServerInfo(QString& host, int& port)
{
    host = m_LampBasicServerHost;
    port = m_LampBasicServerPort;
}

void CSystemSettingDlg::getRealTimeDataServerInfo(QString& host, int& port)
{
    host = m_RealServerHost;
    port = m_RealServerPort;
}

void CSystemSettingDlg::getHistoryDataServerInfo(QString& host, int& port)
{
    host = m_HistServerHost;
    port = m_HistServerPort;
}
void CSystemSettingDlg::getWSO2ServerUserPwd( QString &user, QString &passwd)
{
    user = m_WSO2Username;
    passwd = m_WSO2Password;
}

void CSystemSettingDlg::getWSO2ServerHostAndPort(QString& host, int& httpport, int& mqttport)
{
    host = m_WSO2ServerHost;
    httpport = m_WSO2ServerHTTPPort;
    mqttport = m_WSO2ServerMQTTPort;
}

QString CSystemSettingDlg::getServerDomainName()
{
    int index = m_WSO2Username.indexOf('@');
    if(index == -1)
    {
        return "carbon.super";
    }
    else
    {
        return m_WSO2Username.right(m_WSO2Username.length()-index-1);
    }
}
//生成ApplicationKey
void CSystemSettingDlg::generateApplicationKey()
{
    //将用户名和密码进行base64编码，生成baseStr
    QByteArray  byteArr = QString("%1:%2").arg(m_WSO2Username).arg(m_WSO2Password).toUtf8();
    qDebug()<<"byteArr:"<<byteArr;
    QByteArray baseStr = byteArr.toBase64();
    qDebug()<<"baseStr:"<<baseStr;

    //发送POST请求，生成CLIENT_ID和CLIENT_SECRET
    QString params = "{\"applicationName\": \"DefaultApplication\", \"tags\": [\"device_management\"] }";
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QString headerData = "Basic " + baseStr;
    request.setRawHeader("Authorization", headerData.toUtf8());

    QString strUrl = QString("http://%1:%2/api-application-registration/register").arg(m_WSO2ServerHost).arg(m_WSO2ServerHTTPPort);
    request.setUrl(QUrl(strUrl));

    QNetworkAccessManager  manager;
    QString clientId = "";
    QString clientSecret = "";
    QNetworkReply* reply = manager.post(request, params.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyData = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            QVariantMap map = document.toVariant().toMap();
            clientId = map["client_id"].toString();
            clientSecret = map["client_secret"].toString();
        }
    }

    byteArr = QString("%1:%2").arg(clientId).arg(clientSecret).toUtf8();
    m_applicationKey = QString(byteArr.toBase64());
    qDebug()<<"clientId:"<<clientId<<"clientSecret:"
           <<clientSecret<<"m_applicationKey:"<<m_applicationKey;

    reply->deleteLater();
    reply = Q_NULLPTR;
}

void CSystemSettingDlg::generateAccessToken()
{

    QString strPara = QString("grant_type=password&username=%1&password=%2&"
                      "scope=perm:sign-csr perm:admin:devices:view perm:admin:topics:view "
                      "perm:roles:add perm:roles:add-users perm:roles:update "
                      "perm:roles:permissions perm:roles:details perm:roles:view "
                      "perm:roles:create-combined-role perm:roles:delete "
                      "perm:dashboard:vulnerabilities perm:dashboard:non-compliant-count "
                      "perm:dashboard:non-compliant perm:dashboard:by-groups "
                      "perm:dashboard:device-counts perm:dashboard:feature-non-compliant "
                      "perm:dashboard:count-overview perm:dashboard:filtered-count "
                      "perm:dashboard:details perm:get-activity perm:devices:delete "
                      "perm:devices:applications perm:devices:effective-policy "
                      "perm:devices:compliance-data perm:devices:features "
                      "perm:devices:operations perm:devices:search perm:devices:details "
                      "perm:devices:update perm:devices:view perm:view-configuration "
                      "perm:manage-configuration perm:policies:remove perm:policies:priorities "
                      "perm:policies:deactivate perm:policies:get-policy-details "
                      "perm:policies:manage perm:policies:activate perm:policies:update "
                      "perm:policies:changes perm:policies:get-details perm:users:add "
                      "perm:users:details perm:users:count perm:users:delete perm:users:roles "
                      "perm:users:user-details perm:users:credentials perm:users:search "
                      "perm:users:is-exist perm:users:update perm:users:send-invitation "
                      "perm:admin-users:view perm:groups:devices perm:groups:update "
                      "perm:groups:add perm:groups:device perm:groups:devices-count "
                      "perm:groups:remove perm:groups:groups perm:groups:groups-view "
                      "perm:groups:share perm:groups:count perm:groups:roles "
                      "perm:groups:devices-remove perm:groups:devices-add perm:groups:assign "
                      "perm:device-types:features perm:device-types:types "
                      "perm:applications:install perm:applications:uninstall "
                      "perm:admin-groups:count perm:admin-groups:view "
                      "perm:notifications:mark-checked perm:notifications:view "
                      "perm:admin:certificates:delete perm:admin:certificates:details "
                      "perm:admin:certificates:view perm:admin:certificates:add "
                      "perm:admin:certificates:verify perm:admin perm:devicetype:deployment "
                      "perm:device-types:events perm:device-types:events:view "
                      "perm:admin:device-type perm:device:enroll perm:geo-service:analytics-view "
                      "perm:geo-service:alerts-manage perm:sign-csr perm:admin:devices:view "
                      "perm:roles:add perm:roles:add-users perm:roles:update "
                      "perm:roles:permissions perm:roles:details perm:roles:view "
                      "perm:roles:create-combined-role perm:roles:delete "
                      "perm:get-activity perm:devices:delete perm:devices:applications "
                      "perm:devices:effective-policy perm:devices:compliance-data "
                      "perm:devices:features perm:devices:operations perm:devices:search "
                      "perm:devices:details perm:devices:update perm:devices:view "
                      "perm:view-configuration perm:manage-configuration perm:policies:remove "
                      "perm:policies:priorities perm:policies:deactivate "
                      "perm:policies:get-policy-details perm:policies:manage "
                      "perm:policies:activate perm:policies:update perm:policies:changes "
                      "perm:policies:get-details perm:users:add perm:users:details "
                      "perm:users:count perm:users:delete perm:users:roles "
                      "perm:users:user-details perm:users:credentials perm:users:search "
                      "perm:users:is-exist perm:users:update perm:users:send-invitation "
                      "perm:admin-users:view perm:groups:devices perm:groups:update "
                      "perm:groups:add perm:groups:device perm:groups:devices-count "
                      "perm:groups:remove perm:groups:groups perm:groups:groups-view "
                      "perm:groups:share perm:groups:count perm:groups:roles "
                      "perm:groups:devices-remove perm:groups:devices-add perm:groups:assign "
                      "perm:device-types:features perm:device-types:types "
                      "perm:applications:install perm:applications:uninstall "
                      "perm:admin-groups:count perm:admin-groups:view "
                      "perm:notifications:mark-checked perm:notifications:view "
                      "perm:admin:certificates:delete perm:admin:certificates:details "
                      "perm:admin:certificates:view perm:admin:certificates:add "
                      "perm:admin:certificates:verify perm:ios:enroll perm:ios:view-device "
                      "perm:ios:apn perm:ios:ldap perm:ios:enterprise-app "
                      "perm:ios:store-application perm:ios:remove-application perm:ios:app-list "
                      "perm:ios:profile-list perm:ios:lock perm:ios:enterprise-wipe "
                      "perm:ios:device-info perm:ios:restriction perm:ios:email "
                      "perm:ios:cellular perm:ios:applications perm:ios:wifi perm:ios:ring "
                      "perm:ios:location perm:ios:notification perm:ios:airplay "
                      "perm:ios:caldav perm:ios:cal-subscription perm:ios:passcode-policy "
                      "perm:ios:webclip perm:ios:vpn perm:ios:per-app-vpn "
                      "perm:ios:app-to-per-app-vpn perm:ios:app-lock perm:ios:clear-passcode "
                      "perm:ios:remove-profile perm:ios:get-restrictions perm:ios:wipe-data "
                      "perm:admin perm:android:enroll perm:android:wipe perm:android:ring "
                      "perm:android:lock-devices perm:android:configure-vpn "
                      "perm:android:configure-wifi perm:android:enroll "
                      "perm:android:uninstall-application perm:android:manage-configuration "
                      "perm:android:location perm:android:install-application perm:android:mute "
                      "perm:android:change-lock-code perm:android:blacklist-applications "
                      "perm:android:set-password-policy perm:android:encrypt-storage "
                      "perm:android:clear-password perm:android:enterprise-wipe "
                      "perm:android:info perm:android:view-configuration "
                      "perm:android:upgrade-firmware perm:android:set-webclip "
                      "perm:android:send-notification perm:android:disenroll "
                      "perm:android:update-application perm:android:unlock-devices "
                      "perm:android:control-camera perm:android:reboot perm:android:logcat "
                      "perm:device:enroll perm:device:disenroll perm:device:modify "
                      "perm:device:operations perm:device:publish-event").arg(m_WSO2Username).arg(m_WSO2Password);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    QString headerData = "Basic " + m_applicationKey;
    request.setRawHeader("Authorization", headerData.toUtf8());
    QString strUrl = QString("http://%1:%2/token").arg(m_WSO2ServerHost).arg(m_WSO2ServerHTTPPort);
    request.setUrl(QUrl(strUrl));
    QNetworkAccessManager  manager;
    QNetworkReply* reply = manager.post(request, strPara.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(reply->error() == QNetworkReply::NoError)
    {
        //qDebug()<<"11111111111111111111";
        QByteArray replyData = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            //qDebug()<<"2222222222222222222";
            QVariantMap map = document.toVariant().toMap();
            m_accessToken = map["access_token"].toString();
            m_refreshToken = map["refresh_token"].toString();
        }
    }
    qDebug()<<"m_accessToken:"<<m_accessToken<<"m_refreshToken:"<<m_refreshToken;

    reply->deleteLater();
    reply = Q_NULLPTR;
}

QString CSystemSettingDlg::getAccessToken()
{
    return m_accessToken;
}

QString CSystemSettingDlg::getRefreshToken()
{
    return m_refreshToken;
}

void CSystemSettingDlg::refreshAccessToken()
{
    QString params = QString("grant_type=refresh_token&refresh_token=%1&scope=PRODUCTION").arg(m_refreshToken);
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    QString headerData = "Basic " + m_applicationKey;
    request.setRawHeader("Authorization", headerData.toUtf8());

    QString strUrl = QString("http://%1:%2/token").arg(m_WSO2ServerHost).arg(m_WSO2ServerHTTPPort);
    request.setUrl(QUrl(strUrl));

    QNetworkAccessManager  manager;
    QNetworkReply* reply = manager.post(request, params.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"refreshAccessToken success!";
        QByteArray replyData = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        if(!document.isNull())
        {
            QVariantMap map = document.toVariant().toMap();
            m_accessToken = map["access_token"].toString();
            m_refreshToken = map["refresh_token"].toString();
        }
    }
    qDebug()<<"refreshAccessToken:"<<m_accessToken<<"refreshAccessToken:"<<m_refreshToken;
    reply->deleteLater();
    reply = Q_NULLPTR;
}
