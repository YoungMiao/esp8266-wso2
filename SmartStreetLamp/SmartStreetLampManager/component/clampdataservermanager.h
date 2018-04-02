#ifndef CLAMPDATASERVERMANAGER_H
#define CLAMPDATASERVERMANAGER_H
#include "include/lampinfo.h"
#include <QObject>
#include <QVector>
#define gDataServerConfig CLampDataServerManager::getInstance()
class QSqlDatabase;
class QNetworkAccessManager;
class QNetworkRequest;
class CLampDataServerManager: public QObject
{
    Q_OBJECT
public:
    ~CLampDataServerManager();
    static CLampDataServerManager* getInstance();

private:

    QString m_dataServerIP;
    int m_dataServerPort;

    explicit CLampDataServerManager(QObject* parent = 0);
    CLampDataServerManager(const CLampDataServerManager&);
    CLampDataServerManager& operator=(const CLampDataServerManager);
    static CLampDataServerManager* m_LampDataServer;

    QNetworkAccessManager* m_networkManager;
    QNetworkRequest* m_networkRequest;

public:
    QVector<lampInfoStru> getLampDataInfo();
    QVector<lampInfoStru> getLampInfoByTableName(QString roadTableName);

    bool checkInputUsername(QString user, QString passwd, int& userID);

};

#endif // CLAMPDATASERVERMANAGER_H
