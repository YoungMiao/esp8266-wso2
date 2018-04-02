#ifndef CLAMPREALTIMEDATADIALOG_H
#define CLAMPREALTIMEDATADIALOG_H

#include <QDialog>
#include "include/lampinfo.h"
class QTableWidgetItem;
class CkaaSubject;
namespace Ui {
class CLampRealTimeDataDialog;
}

class CLampRealTimeDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLampRealTimeDataDialog(QWidget *parent = 0);
    ~CLampRealTimeDataDialog();

private:
    virtual void resizeEvent(QResizeEvent* event);
    void initRealTimeDataTable();
    void parseJsonData(QString strEnvJson);
    void closeEvent(QCloseEvent *);
private:
    Ui::CLampRealTimeDataDialog *ui;
    CkaaSubject *m_kaaRegistServer;
    QString m_lampSerialNum;
    lampEnvInfoStru m_realTimeEnvInfo;
public:
    void resetEnvData();
    void setRealTimeData(lampEnvInfoStru&  lampEnvInfo);
    void setKaaRegistServer(CkaaSubject *kaaRegistServer);
    void subscribeRealTimeTopic(QString strLampID);
private Q_SLOTS:
    void onGetRealTimeData(const QString &strTopic, const QString &strEnvInfo);

};

#endif // CLAMPREALTIMEDATADIALOG_H
