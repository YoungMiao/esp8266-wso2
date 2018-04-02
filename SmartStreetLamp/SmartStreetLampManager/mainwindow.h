#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtnRibbonMainWindow.h>
#include "include/lampinfo.h"
#include <QtitanRibbon.h>

class CMapOperation;
class CLampInfoTree;
class CSystemSettingDlg;
class QPushButton;
class CLightingStrategyDialog;
class CkaaSubject;
class MainWindow : public Qtitan::RibbonMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, const QVector<lampInfoStru>& lampInfoVecVec = QVector<lampInfoStru>());
    ~MainWindow();

private:
    //Ui::MainWindow *ui;
    //MapView *m_View;
    CMapOperation  *m_MapOperation;
    CLampInfoTree  *m_LampInfoOp;
    CSystemSettingDlg  *m_sysSettingDlg;
    QPushButton  *m_sysSettingButton;
    lampEnvInfoStru m_lampEnvInfo;
    //设置照明策略
    CLightingStrategyDialog* m_LightingStrategyDlg;

    CkaaSubject* m_kaaRegistServer;
    QVector<lampInfoStru> m_lampInfoVecVec;
private:
    void createRibbonBar();
    void createIntelligentStrategyPage(QString strStrategy);
    void createLampDataOperationPage(QString LampDataStr);
    void parseJsonData(QString strJson);


private Q_SLOTS:
    void onActionImport();
    void onActionExport();
    void showSysSettingDlg();
    //void onGetRealTimeData(const QString&, const QString&);
    void onActionBasicStrategyHandler();
    void onActionHolidayStrategyHandler();
};

#endif // MAINWINDOW_H
