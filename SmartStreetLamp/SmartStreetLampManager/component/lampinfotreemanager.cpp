#include "lampinfotreemanager.h"
#include <QAction>
#include <QMenu>
#include <QDebug>


#include "setting/csystemsettingdlg.h"
#include "ui/clampbasicinfodialog.h"
#include "ui/clamphistorydatadialog.h"
#include "ui/clamprealtimedatadialog.h"

CLampInfoTree::CLampInfoTree(QWidget* parent,CkaaSubject* kaaRegistServer,
                             const QVector<lampInfoStru>& lampInfoVecVec):
    QTreeWidget(parent)
{
    createActions();
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(onCustomContextMenuRequestedHandler(const QPoint&)));
    m_LampBasicInfoDlg = Q_NULLPTR;
    m_LampHistoryInfoDlg = Q_NULLPTR;
    m_LampRealtimeInfoDlg = Q_NULLPTR;
    m_curSelectTreeItem = Q_NULLPTR;

    m_kaaRegistServer = kaaRegistServer;
    m_lampInfoVecVec = lampInfoVecVec;

    InitLampInfoTree();

}

CLampInfoTree::~CLampInfoTree()
{

}


void CLampInfoTree::InitLampInfoTree()
{

    this->setColumnCount(2);
    this->setColumnHidden(1, true);
    QVector<lampInfoStru>::iterator  iter1 = m_lampInfoVecVec.begin();
    while(iter1 != m_lampInfoVecVec.end())
    {
        QString city = (*iter1).belongCityName;
        QTreeWidgetItem* cityItem = addTreeWidgetItem(Q_NULLPTR, city);
        //cityItem->setCheckState(0, Qt::Unchecked);

        QString region = (*iter1).belongRegionName;
        QTreeWidgetItem* regItem = addTreeWidgetItem(cityItem, region);
        //regItem->setCheckState(0, Qt::Unchecked);

        QString street = (*iter1).belongStreetName;
        QTreeWidgetItem* streetItem = addTreeWidgetItem(regItem, street);
        //streetItem->setCheckState(0, Qt::Unchecked);
        QString lampName = (*iter1).lampName;
        QString serialStr = (*iter1).lampSerialNumber;
        QStringList strList;
        strList.append(lampName);
        strList.append(serialStr);
        QTreeWidgetItem* lampItem = new QTreeWidgetItem(strList);
        //lampItem->setCheckState(0, Qt::Unchecked);
        streetItem->addChild(lampItem);
        ++iter1;
    }

    this->expandAll();
    this->setContextMenuPolicy(Qt::CustomContextMenu);

}
//在当前的Item下插入新节点，并返回该节点
QTreeWidgetItem* CLampInfoTree::addTreeWidgetItem(QTreeWidgetItem *item, QString strName)
{
    //遍历tree，如果找到名字相同的的Item，则返回该Item，否则插入该Item，并返回该Item
    QTreeWidgetItemIterator  iter(this);
    while(*iter)
    {
        if((*iter)->text(0) == strName)
        {
            return *iter;
        }

        ++iter;
    }
    //没找到相同名字的Item，则插入该Item
    QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(strName));
    if(item == Q_NULLPTR)
    {
       addTopLevelItem(newItem);
    }
    else
    {
        item->addChild(newItem);
    }

    return newItem;
}


void CLampInfoTree::onCustomContextMenuRequestedHandler(const QPoint& point)
{

    m_curSelectTreeItem = this->itemAt(point);
    if(m_curSelectTreeItem == Q_NULLPTR)
    {
        return;
    }

    QMenu * popMenu = new QMenu(this);
    if(m_curSelectTreeItem->child(0) == Q_NULLPTR)
    {
        popMenu->addAction(m_actShowLampDetail);
        popMenu->addAction(m_actShowLampRealTimeData);
        popMenu->addAction(m_actShowHistoryData);
    }

    popMenu->exec(QCursor::pos());
}

void CLampInfoTree::createActions()
{
    m_actShowLampDetail = new QAction(QString::fromWCharArray(L"显示路灯详细信息"));
    connect(m_actShowLampDetail, SIGNAL(triggered(bool)),this, SLOT(actShowLampDetailHandler()));
    m_actShowHistoryData = new QAction(QString::fromWCharArray(L"显示历史数据"));
    connect(m_actShowHistoryData, SIGNAL(triggered(bool)),this, SLOT(actShowLampHistoryDataHandle()));
    m_actShowLampRealTimeData = new QAction(QString::fromWCharArray(L"显示实时数据"));
    connect(m_actShowLampRealTimeData, SIGNAL(triggered(bool)),this, SLOT(actShowLampRealTimeDataHandle()));
}



lampInfoStru CLampInfoTree::getLampInfoByID(QString lampID)
{
    lampInfoStru lampInfo;
    for(int i = 0; i < m_lampInfoVecVec.size(); i++)
    {
        if(m_lampInfoVecVec[i].lampSerialNumber == lampID)
        {
            lampInfo = m_lampInfoVecVec[i];
            return lampInfo;
        }
    }
    return lampInfo;
}

//显示路灯的详细信息
void CLampInfoTree::actShowLampDetailHandler()
{
    QString lampSerialNum = m_curSelectTreeItem->text(1);
    lampInfoStru lampInfo = getLampInfoByID(lampSerialNum);
    if(m_LampBasicInfoDlg == Q_NULLPTR)
    {
        m_LampBasicInfoDlg = new CLampBasicInfoDialog(this);
    }
    m_LampBasicInfoDlg->setLampBasicInfo(lampInfo);
    m_LampBasicInfoDlg->exec();
}
//显示路灯历史数据
void CLampInfoTree::actShowLampHistoryDataHandle()
{
    QString lampSerialNum = m_curSelectTreeItem->text(1);
    if(m_LampHistoryInfoDlg == Q_NULLPTR)
    {
        m_LampHistoryInfoDlg = new CLampHistoryDataDialog(this);
    }
    m_LampHistoryInfoDlg->getLampSerialNumber(lampSerialNum);
    m_LampHistoryInfoDlg->exec();

}

//显示路灯实时数据
void CLampInfoTree::actShowLampRealTimeDataHandle()
{
    QString lampSerialNum = m_curSelectTreeItem->text(1);
    if(m_LampRealtimeInfoDlg == Q_NULLPTR)
    {
        m_LampRealtimeInfoDlg = new CLampRealTimeDataDialog(this);
        m_LampRealtimeInfoDlg->setKaaRegistServer(m_kaaRegistServer);
    }
    m_LampRealtimeInfoDlg->subscribeRealTimeTopic(lampSerialNum);
    m_LampRealtimeInfoDlg->exec();

}


