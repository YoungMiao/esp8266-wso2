#ifndef SMARTSTREETCONTROLKAA_H
#define SMARTSTREETCONTROLKAA_H
#include "apibase.h"
#include "smartstreetcontrolbase.h"
class CSmartStreetControlKaa : public APIBase,
                                public CSmartStreetControlBase
{
    Q_OBJECT
public:
    explicit CSmartStreetControlKaa(QObject *parent = 0);

private:
    void controlByFile(const QString& strNotificationPath,
                       const QString& strFilePath, const QString& strLampID=QString());

    void controlByConfig(const QString& strGroupID, const QString& strStrategy);
    void ActiveConfigById(const QString& configID);
public:
    virtual QNetworkRequest createRequest(const QUrl &url) const;
public:

    //根据路灯的ID开灯
    virtual bool TurnOnLampByLampid(const QString& lampId);
    virtual bool TurnOffLampByLampid(const QString& lampId){return true;}
    //根据策略开关灯
    virtual bool TurnOnLampByStrategy(const QString& streetID,
                                      const QString& strategyName);
    virtual bool TurnOffLampByStrategy(const QString& streetID,
                                       const QString& strategyName);

    //固件管理
    //升级固件
    virtual bool UpgradeFirmware(const QString& streetID,
                            const QString& FirmwareInfo){return true;}
    //恢复出厂设置
    virtual bool FactoryReset(const QString& streetID){return true;}

    //重新启动
    virtual bool reboot(const QString& streetID){return true;}
signals:

public slots:
private:
    QString mStrConfigPath;
    QVariantMap mHeaderMap;


    QString strKaaControlIp;
    QString strKaaControlPort;
    QString strKaaUser;
    QString strKaaPasswd;
    QString strApplicationID;
    QVariantList groupIDList;
    QString strConfigSchemaId;
};

#endif // SMARTSTREETCONTROLKAA_H
