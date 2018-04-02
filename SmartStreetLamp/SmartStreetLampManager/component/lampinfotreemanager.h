#ifndef LAMPINFOTREEMANAGER_H
#define LAMPINFOTREEMANAGER_H
#include <QTreeWidget>
#include "include/lampinfo.h"

class CLampBasicInfoDialog;
class CLampHistoryDataDialog;
class CLampRealTimeDataDialog;

class CkaaSubject;
class CLampInfoTree:public QTreeWidget
{
    Q_OBJECT
public:
    explicit CLampInfoTree(QWidget* parent = 0, CkaaSubject* kaaRegistServer = Q_NULLPTR,
                          const QVector<lampInfoStru>& lampInfoVecVec = QVector<lampInfoStru>());
    ~CLampInfoTree();

private:

    //单个路灯只显示基础信息，实时数据，历史数据
    QAction* m_actShowLampDetail;
    QAction* m_actShowLampRealTimeData;
    QAction* m_actShowHistoryData;

    QVector<lampInfoStru>  m_lampInfoVecVec; //保存所有路灯的数据，里面的vector保存每条街道的路灯。

    QTreeWidgetItem *m_curSelectTreeItem;
    //QString m_curSelectTreeItemStr;     //例如：路灯0001
    //QString m_curSelectTreeItemSerailNum; //例如：01010100010001

private:

    CLampBasicInfoDialog* m_LampBasicInfoDlg;
    CLampHistoryDataDialog* m_LampHistoryInfoDlg;
    CLampRealTimeDataDialog* m_LampRealtimeInfoDlg;
    CkaaSubject *m_kaaRegistServer;

private:

    lampInfoStru getLampInfoByID(QString lampID);
    QTreeWidgetItem* addTreeWidgetItem(QTreeWidgetItem *item, QString strName);
    void createActions();

    void InitLampInfoTree();

private Q_SLOTS:
    //树点击右键操作
    void onCustomContextMenuRequestedHandler(const QPoint& point);

    //显示路灯的详细信息
    void actShowLampDetailHandler();
    //显示路灯历史数据
    void actShowLampHistoryDataHandle();
    //显示路灯实时数据
    void actShowLampRealTimeDataHandle();

};

#endif // LAMPINFOTREEMANAGER_H
