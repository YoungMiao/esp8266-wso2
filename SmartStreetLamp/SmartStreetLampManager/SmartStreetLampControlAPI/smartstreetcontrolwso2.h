#ifndef CSMARTSTREETCONTROLWSO2_H
#define CSMARTSTREETCONTROLWSO2_H
#include "apibase.h"
#include "smartstreetcontrolbase.h"

class CSmartStreetControlWSO2: public APIBase,
        public CSmartStreetControlBase
{
    Q_OBJECT
public:
    explicit CSmartStreetControlWSO2(QObject *parent = 0);

    //
    bool TurnOnLampByLampidList(const QStringList& lampIdList);
    bool TurnOffLampByLampidList(const QStringList& lampIdList);
     //根据路灯的ID开灯
    virtual bool TurnOnLampByLampid(const QString& lampId){return true;}
    virtual bool TurnOffLampByLampid(const QString& lampId){return true;}
    //根据策略开关灯
    virtual bool TurnOnLampByStrategy(const QString& streetID,
                                      const QString& strategyName){return true;}
    virtual bool TurnOffLampByStrategy(const QString& streetID,
                                       const QString& strategyName){return true;}

    //固件管理
    //升级固件
    virtual bool UpgradeFirmware(const QString& streetID,
                            const QString& FirmwareInfo){return true;}
    //恢复出厂设置
    virtual bool FactoryReset(const QString& streetID){return true;}

    //重新启动
    virtual bool reboot(const QString& streetID){return true;}

};

#endif // CSMARTSTREETCONTROLWSO2_H
