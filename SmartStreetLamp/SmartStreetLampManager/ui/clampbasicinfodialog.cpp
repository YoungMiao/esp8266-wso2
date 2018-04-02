#include "clampbasicinfodialog.h"
#include "ui_clampbasicinfodialog.h"
#include "setting/csystemsettingdlg.h"
#include <QDebug>
extern QString lampControlWayToString(lampControlWay controlway);

CLampBasicInfoDialog::CLampBasicInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLampBasicInfoDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    initLampBasicInfoTable();

}

CLampBasicInfoDialog::~CLampBasicInfoDialog()
{
    delete ui;
}

void CLampBasicInfoDialog::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPixmap  pixmap(":/resource/lamp.png");
    QPixmap newpix = pixmap.scaled(ui->widget_lampPicture->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    ui->widget_lampPicture->setAutoFillBackground(true);

    QPalette  palette = ui->widget_lampPicture->palette();

    palette.setBrush(QPalette::Window, QBrush(newpix));
    ui->widget_lampPicture->setPalette(palette);
}


void CLampBasicInfoDialog::initLampBasicInfoTable()
{
    ui->tableWidget_basicInfo->setColumnCount(4);
    ui->tableWidget_basicInfo->setRowCount(7);
    ui->tableWidget_basicInfo->horizontalHeader()->stretchLastSection();
    ui->tableWidget_basicInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_basicInfo->horizontalHeader()->setVisible(false);
    ui->tableWidget_basicInfo->verticalHeader()->setVisible(false);
    ui->tableWidget_basicInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_basicInfo->setSelectionBehavior(QAbstractItemView::SelectItems);

    for(int i = 0; i< 7; i++)
    {
        QTableWidgetItem* item1 = new QTableWidgetItem();
        QTableWidgetItem* item2 = new QTableWidgetItem();
        QTableWidgetItem* item3 = new QTableWidgetItem();
        QTableWidgetItem* item4 = new QTableWidgetItem();
        ui->tableWidget_basicInfo->setItem(i, 0, item1);
        ui->tableWidget_basicInfo->setItem(i, 1, item2);
        ui->tableWidget_basicInfo->setItem(i, 2, item3);
        ui->tableWidget_basicInfo->setItem(i, 3, item4);
    }

    ui->tableWidget_basicInfo->item(0,0)->setText(QString::fromWCharArray(L"路灯ID"));
    ui->tableWidget_basicInfo->item(0,2)->setText(QString::fromWCharArray(L"路灯编号"));
    ui->tableWidget_basicInfo->item(1,0)->setText(QString::fromWCharArray(L"路灯经度"));
    ui->tableWidget_basicInfo->item(1,2)->setText(QString::fromWCharArray(L"路灯纬度"));
    ui->tableWidget_basicInfo->item(2,0)->setText(QString::fromWCharArray(L"所属街道"));
    ui->tableWidget_basicInfo->item(2,2)->setText(QString::fromWCharArray(L"所属回路"));
    ui->tableWidget_basicInfo->item(3,0)->setText(QString::fromWCharArray(L"所属配电箱"));
    ui->tableWidget_basicInfo->item(3,2)->setText(QString::fromWCharArray(L"控制方式"));
    ui->tableWidget_basicInfo->item(4,0)->setText(QString::fromWCharArray(L"灯型"));
    ui->tableWidget_basicInfo->item(4,2)->setText(QString::fromWCharArray(L"灯具数量"));
    ui->tableWidget_basicInfo->item(5,0)->setText(QString::fromWCharArray(L"灯杆高"));
    ui->tableWidget_basicInfo->item(5,2)->setText(QString::fromWCharArray(L"生产厂家"));
    ui->tableWidget_basicInfo->item(6,0)->setText(QString::fromWCharArray(L"建设单位"));
    ui->tableWidget_basicInfo->item(6,2)->setText(QString::fromWCharArray(L"建设时间"));

}


void CLampBasicInfoDialog::setLampBasicInfo(lampInfoStru lampInfo)
{
    QString strControl = lampControlWayToString(lampInfo.controlWay);
    ui->tableWidget_basicInfo->item(0,1)->setText(lampInfo.lampName);
    ui->tableWidget_basicInfo->item(0,3)->setText(lampInfo.lampSerialNumber);
    ui->tableWidget_basicInfo->item(1,1)->setText(QString::number(lampInfo.longitude,'f', 6));
    ui->tableWidget_basicInfo->item(1,3)->setText(QString::number(lampInfo.latitude,'f', 6));
    ui->tableWidget_basicInfo->item(2,1)->setText(lampInfo.belongStreetName);
    ui->tableWidget_basicInfo->item(2,3)->setText(lampInfo.belongCircle);
    ui->tableWidget_basicInfo->item(3,1)->setText(lampInfo.switchBox);
    ui->tableWidget_basicInfo->item(3,3)->setText(strControl);
    ui->tableWidget_basicInfo->item(4,1)->setText(lampInfo.lampType);
    ui->tableWidget_basicInfo->item(4,3)->setText(QString::number(lampInfo.lampCount));
    ui->tableWidget_basicInfo->item(5,1)->setText(QString::number(lampInfo.lampHeight));
    ui->tableWidget_basicInfo->item(5,3)->setText(lampInfo.manufacturer);
    ui->tableWidget_basicInfo->item(6,1)->setText(lampInfo.department);
    ui->tableWidget_basicInfo->item(6,3)->setText(lampInfo.buildTime.toString("yyyy-MM-dd"));
}
