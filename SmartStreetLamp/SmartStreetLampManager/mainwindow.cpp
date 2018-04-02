#include "mainwindow.h"
#include <QTreeView>
#include <QDockWidget>
#include <QPushButton>
#include <QtnRibbonBar.h>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include "component/mapoperation.h"
#include "component/lampinfotreemanager.h"
#include "setting/csystemsettingdlg.h"
#include "setting/clightingstrategydialog.h"
#include "SmartStreetLampControlAPI/kaasubjectmqtt.h"

MainWindow::MainWindow(QWidget *parent, const QVector<lampInfoStru>& lampInfoVecVec) :
    Qtitan::RibbonMainWindow(parent)
{
    this->setWindowTitle(QString::fromWCharArray(L"智慧路灯管理系统"));
    this->setWindowIcon(QIcon(":/resource/images/app.png"));

    m_lampInfoVecVec = lampInfoVecVec;

    QAction* actionSysSetting = new QAction(QString::fromWCharArray(L"系统设置"));
    actionSysSetting->setIcon(QIcon(":/resource/images/settingIcon.png"));
    ribbonBar()->addAction(actionSysSetting, Qt::ToolButtonIconOnly);
    m_sysSettingDlg = gSysSetting;
    connect(actionSysSetting, SIGNAL(triggered(bool)),this, SLOT(showSysSettingDlg()));

    m_LightingStrategyDlg = new CLightingStrategyDialog(this);
    //注册kaa服务
    m_kaaRegistServer = new CKaaSubjectMqtt;
    QString mqttServer;
    int mqttPort;
    gSysSetting->getRealTimeDataServerInfo(mqttServer, mqttPort);

    QVariantMap mqttMap;
    mqttMap.insert("ip", mqttServer);
    mqttMap.insert("port", mqttPort);
    qDebug()<<"mqttServer is: "<<mqttServer<<"mqttPort is "<<mqttPort<<endl;
    m_kaaRegistServer->registerPublisher(mqttMap);

    m_MapOperation = new CMapOperation(this, QApplication::applicationDirPath()+"/map/LampInfo.qgs", true, m_kaaRegistServer);
    this->setCentralWidget( m_MapOperation );
    m_MapOperation->getLampLightingStategy(m_LightingStrategyDlg);

    m_LampInfoOp = new CLampInfoTree(this, m_kaaRegistServer, m_lampInfoVecVec);
    m_LampInfoOp->setHeaderHidden(true);
    QDockWidget* lampInfoDockWidget = new QDockWidget(QString::fromWCharArray(L"路灯信息"),this);
    lampInfoDockWidget->setWidget(reinterpret_cast<QWidget*>(m_LampInfoOp));
    lampInfoDockWidget->setFixedWidth(260);
    lampInfoDockWidget->setContentsMargins(0,6,0,6);
    lampInfoDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::LeftDockWidgetArea, lampInfoDockWidget);


    createRibbonBar();

    move(QPoint(0, 0));
    QRect geom = QApplication::desktop()->availableGeometry();
    resize(geom.width(), geom.height());

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::showSysSettingDlg()
{
    m_sysSettingDlg->exec();
}


void MainWindow::createRibbonBar()
{

    if(ribbonBar() != Q_NULLPTR)
    {
        m_MapOperation->createMapOperationPage(ribbonBar(), QString::fromWCharArray(L"地图操作"));
        createIntelligentStrategyPage(QString::fromWCharArray(L"智能策略"));
        createLampDataOperationPage(QString::fromWCharArray(L"路灯数据操作"));
    }

    ribbonBar()->setFrameThemeEnabled(true);

}

void MainWindow::createIntelligentStrategyPage(QString strStrategy)
{
    RibbonPage* strategyPage = ribbonBar()->addPage(strStrategy);
    if(strategyPage != Q_NULLPTR)
    {
        if(RibbonGroup* groupBasicStrategy = strategyPage->addGroup(strStrategy))
        {
            groupBasicStrategy->setOptionButtonVisible();
            QAction* actionBasic = new QAction(QIcon(":/resource/1.png"), QString::fromWCharArray(L"基本策略"));
            groupBasicStrategy->addAction(actionBasic, Qt::ToolButtonTextUnderIcon);
            connect(actionBasic, SIGNAL(triggered(bool)),this, SLOT(onActionBasicStrategyHandler()));

            QAction* actionHoliday = new QAction(QIcon(":/resource/2.png"),QString::fromWCharArray(L"节假日策略"));
            groupBasicStrategy->addAction(actionHoliday, Qt::ToolButtonTextUnderIcon);
            connect(actionHoliday, SIGNAL(triggered(bool)),this, SLOT(onActionHolidayStrategyHandler()));
        }
    }

    return;
}

void MainWindow::createLampDataOperationPage(QString LampDataStr)
{

    RibbonPage* lampDataPage = ribbonBar()->addPage(LampDataStr);
    if(lampDataPage != Q_NULLPTR)
    {
        //add groups
        if(RibbonGroup* groupLampStates = lampDataPage->addGroup(QString::fromWCharArray(L"数据操作")))
        {
            groupLampStates->setOptionButtonVisible();

            QAction* actionImport = new QAction(QIcon(":/resource/1.png"), QString::fromWCharArray(L"导入路灯数据"));
            groupLampStates->addAction(actionImport, Qt::ToolButtonTextUnderIcon);
            connect(actionImport, SIGNAL(triggered()),this, SLOT(onActionImport()));

            QAction* actionExport = new QAction(QIcon(":/resource/2.png"), QString::fromWCharArray(L"导出路灯数据"));
            groupLampStates->addAction(actionExport, Qt::ToolButtonTextUnderIcon);
            connect(actionExport, SIGNAL(triggered()), this, SLOT(onActionExport()));

        }
    }

    return;
}
void MainWindow::onActionBasicStrategyHandler()
{
    if(m_LightingStrategyDlg != Q_NULLPTR)
    {
        m_LightingStrategyDlg->initDisplayTree(m_lampInfoVecVec);
        m_LightingStrategyDlg->exec();
    }
}

void MainWindow::onActionHolidayStrategyHandler()
{

}

void MainWindow::onActionImport()
{
    QString path = QApplication::applicationDirPath()+"/coordinate";
    QFileDialog* fd = new QFileDialog(this, QString::fromWCharArray(L"选择路灯坐标文件"), path, "Lamp Coordinate Files(*.csv)");
    if(fd->exec() == QDialog::Accepted)
    {
        QString fileName = fd->selectedFiles()[0];

    }

}

void MainWindow::onActionExport()
{

}
