#include "clightingstrategydialog.h"
#include "ui_clightingstrategydialog.h"
#include "SmartStreetLampControlAPI/lampcontrolcommanddef.h"
#include "SmartStreetLampControlAPI/smartstreetcontrolwso2.h"
#include <QDebug>
CLightingStrategyDialog::CLightingStrategyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLightingStrategyDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    this->setFixedSize(this->width(),this->height());

    m_streetControl = new CSmartStreetControlWSO2(this);
    qDebug()<<"m_streetControl:"<<m_streetControl;
    initPara();
    connect(ui->region_treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(onItemChangedHandle(QTreeWidgetItem*, int)));

 }

CLightingStrategyDialog::~CLightingStrategyDialog()
{
    delete ui;
}


void CLightingStrategyDialog::initPara()
{

    ui->timeEdit_on->setTime(QTime(6, 0, 0, 0));
    ui->timeEdit_off->setTime(QTime(18, 0, 0, 0));
    m_startTime = ui->timeEdit_on->time();
    m_endTime = ui->timeEdit_off->time();
    QButtonGroup* group = new QButtonGroup;
    group->addButton(ui->radioButton_full);
    group->addButton(ui->radioButton_half);
    group->addButton(ui->radioButton_interval);
    ui->radioButton_full->setChecked(true);
    connect(group, SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(onRadioButtonClicked(QAbstractButton*)));
    connect(ui->pushButton_On,SIGNAL(clicked(bool)),this, SLOT(onLightingOnButtonClicked()));
    connect(ui->pushButton_Off,SIGNAL(clicked(bool)),this, SLOT(onLightingOffButtonClicked()));

    ui->region_treeWidget->setHeaderHidden(true);
}

QTreeWidgetItem* CLightingStrategyDialog::addTreeWidgetItem(QTreeWidgetItem* item, QStringList strList)
{
    //遍历tree，如果找到名字相同的的Item，则返回该Item，否则插入该Item，并返回该Item
    QTreeWidgetItemIterator iter(ui->region_treeWidget);
    while(*iter)
    {
        if((*iter)->text(0) == strList.at(0))
        {
            return*iter;
        }
        ++iter;
    }
    //没有找到同名的item，则插入该item
    QTreeWidgetItem* newItem = new QTreeWidgetItem(strList);
    if(item == Q_NULLPTR)
    {
        ui->region_treeWidget->addTopLevelItem(newItem);
    }
    else
    {
        item->addChild(newItem);
    }

    return newItem;
}

void CLightingStrategyDialog::initDisplayTree(QVector<lampInfoStru> lampInfoVec)
{
    m_lampInfoVec = lampInfoVec;
    ui->region_treeWidget->setColumnCount(2);
    ui->region_treeWidget->setColumnHidden(1, true);
    qDebug()<<"lampInfoVec size:"<<lampInfoVec.size();
    QVector<lampInfoStru>::iterator iter = lampInfoVec.begin();
    while(iter != lampInfoVec.end())
    {
        QString serialStr = (*iter).lampSerialNumber;
        QString city = (*iter).belongCityName;
        QString citySerial = serialStr.left(4);
        QStringList strList;
        strList.append(city);
        strList.append(citySerial);
        QTreeWidgetItem* cityItem = addTreeWidgetItem(Q_NULLPTR, strList);
        cityItem->setCheckState(0, Qt::Unchecked);
        QString region = (*iter).belongRegionName;
        QString regionSerial = serialStr.left(6);
        strList.clear();
        strList.append(region);
        strList.append(regionSerial);
        QTreeWidgetItem* regionItem = addTreeWidgetItem(cityItem, strList);
        regionItem->setCheckState(0, Qt::Unchecked);
        QString street = (*iter).belongStreetName;
        QString streetSerial = serialStr.left(10);
        strList.clear();
        strList.append(street);
        strList.append(streetSerial);
        QTreeWidgetItem* streetItem = addTreeWidgetItem(regionItem, strList);
        streetItem->setCheckState(0, Qt::Unchecked);
        ++iter;
    }

    ui->region_treeWidget->expandAll();
    ui->region_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

//设置树的三态操作
void CLightingStrategyDialog::setChildCheckState(QTreeWidgetItem* item, Qt::CheckState cs)
{
    if(item == Q_NULLPTR) return;

    for(int i = 0; i < item->childCount(); i++)
    {
        QTreeWidgetItem* child = item->child(i);
        if(child->checkState(0) != cs)
        {
            child->setCheckState(0, cs);
        }
    }

    setParentCheckState(item->parent());
}

void CLightingStrategyDialog::setParentCheckState(QTreeWidgetItem* itemParent)
{
    if(itemParent == Q_NULLPTR) return;

    int selectedCnt = 0;
    int childCnt = itemParent->childCount();
    for(int i = 0; i < childCnt; i++)
    {
        QTreeWidgetItem* child = itemParent->child(i);
        if(child->checkState(0) == Qt::Checked)
        {
            selectedCnt ++;
        }
    }

    if(selectedCnt == 0)
    {
        itemParent->setCheckState(0, Qt::Unchecked);
    }
    else if(selectedCnt == childCnt)
    {
        itemParent->setCheckState(0, Qt::Checked);
    }
    else
    {
        itemParent->setCheckState(0, Qt::PartiallyChecked);
    }
}

bool CLightingStrategyDialog::isTopLevelItem(QTreeWidgetItem* item)
{
    if(item == Q_NULLPTR)
    {
        return false;
    }
    if(item->parent() == Q_NULLPTR)
    {
        return true;
    }
    return false;
}

void CLightingStrategyDialog::onItemChangedHandle(QTreeWidgetItem* item , int column)
{
    if(column != 0 || item == Q_NULLPTR)
    {
        return;
    }

    if(Qt::PartiallyChecked != item->checkState(0))
    {
        setChildCheckState(item, item->checkState(0));
    }
    if(Qt::PartiallyChecked == item->checkState(0))
    {
        if(!isTopLevelItem(item))
        {
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        }
    }

}

//遍历树，返回所有已勾选的节点的ID列表
void CLightingStrategyDialog::getAllSelectedItem(QStringList& serialNumList)
{

    QTreeWidgetItemIterator iter(ui->region_treeWidget);
    while(*iter)
    {
        if((*iter)->checkState(0) == Qt::Checked && (*iter)->childCount() == 0)
        {
            serialNumList.append((*iter)->text(1));
        }
        ++iter;
    }
}

void CLightingStrategyDialog::onRadioButtonClicked(QAbstractButton* button)
{

    //qDebug()<<"m_OnOffStrategy is: "<<m_OnOffStrategy;
}

QStringList CLightingStrategyDialog::getDeviceIdList()
{
    QStringList deviceIdList;
    for(int i = 0; i < 10; i++)
    {
        QString strLampId = m_lampInfoVec[i].lampSerialNumber.right(3);
        deviceIdList.append(strLampId);
    }

    //qDebug()<<"deviceIdList:"<<deviceIdList;
    return deviceIdList;
}



void CLightingStrategyDialog::onLightingOnButtonClicked()
{
    QStringList deviceList = getDeviceIdList();
    m_streetControl->TurnOnLampByLampidList(deviceList);

}

void CLightingStrategyDialog::onLightingOffButtonClicked()
{
    QStringList deviceList = getDeviceIdList();
    m_streetControl->TurnOffLampByLampidList(deviceList);
}

