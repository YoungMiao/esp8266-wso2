#ifndef CSMARTSTREETCONTROLBASE_H
#define CSMARTSTREETCONTROLBASE_H

#include <QString>

class CSmartStreetControlBase
{
public:
    CSmartStreetControlBase();
public:
    //
  virtual  bool TurnOnLampByLampidList(const QStringList& lampIdList)=0;
   virtual bool TurnOffLampByLampidList(const QStringList& lampIdList)=0;

//根据路灯的ID开灯
virtual bool TurnOnLampByLampid(const QString& lampId) = 0;
virtual bool TurnOffLampByLampid(const QString& lampId) = 0;
//根据策略开关灯
virtual bool TurnOnLampByStrategy(const QString& streetID,
                                  const QString& strategyName)= 0;
virtual bool TurnOffLampByStrategy(const QString& streetID,
                                   const QString& strategyName)= 0;

//固件管理
//升级固件
virtual bool UpgradeFirmware(const QString& streetID,
                        const QString& FirmwareInfo)= 0;
//恢复出厂设置
virtual bool FactoryReset(const QString& streetID)= 0;

//重新启动
virtual bool reboot(const QString& streetID)= 0;

};

#endif // CSMARTSTREETCONTROLBASE_H
