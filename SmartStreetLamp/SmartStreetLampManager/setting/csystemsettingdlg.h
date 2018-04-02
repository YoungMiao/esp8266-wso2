#ifndef CSYSTEMSETTINGDLG_H
#define CSYSTEMSETTINGDLG_H

#include <QDialog>

class QListWidgetItem;

#define gSysSetting CSystemSettingDlg::getInstance()

namespace Ui {
class CSystemSettingDlg;
}

class CSystemSettingDlg: public QDialog
{
    Q_OBJECT
public:
    ~CSystemSettingDlg();
    void getLampBasicServerInfo(QString& host,int& port);
    void getRealTimeDataServerInfo(QString& host, int& port);
    void getHistoryDataServerInfo(QString &host, int& port);
    void getWSO2ServerUserPwd(QString& user, QString& passwd);
     QString getServerDomainName();
    void getWSO2ServerHostAndPort(QString& host, int& httpport, int& mqttport);
    static CSystemSettingDlg* getInstance();
    QString getAccessToken();
    QString getRefreshToken();
    void refreshAccessToken();
    void generateApplicationKey();
    void generateAccessToken();

private:
    explicit CSystemSettingDlg();
    CSystemSettingDlg(const CSystemSettingDlg&);
    CSystemSettingDlg& operator =(const CSystemSettingDlg&);
    static CSystemSettingDlg* m_sysSettingDlg;

private:

    Ui::CSystemSettingDlg* ui;

    QString m_LampBasicServerHost;
    int m_LampBasicServerPort;

    QString m_RealServerHost;
    int m_RealServerPort;

    QString m_HistServerHost;
    int m_HistServerPort;

    QString m_WSO2ServerHost;
    QString m_WSO2Username;
    QString m_WSO2Password;
    int m_WSO2ServerHTTPPort;
    int m_WSO2ServerHTTPSPort;
    int m_WSO2ServerMQTTPort;
    QString m_applicationKey;
    QString m_accessToken;
    QString m_refreshToken;

 private:
    //初始化界面
    void initSystemSetting();

    //重置界面值
    void resetSettingParas();
    void initGeneralPagePara();
    void initSystemPagePara();
    void initDataSourcePagePara();

private Q_SLOTS:
    //保存界面设置的值
    void saveSettingDatas();
    void showTabInfo(QListWidgetItem* curItem);
};

#endif // CSYSTEMSETTINGDLG_H
