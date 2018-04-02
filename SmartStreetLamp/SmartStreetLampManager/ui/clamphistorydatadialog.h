#ifndef CLAMPHISTORYDATADIALOG_H
#define CLAMPHISTORYDATADIALOG_H

#include <QDialog>
class CSSLampHistorydataBase;
class CHistoryEnvDataDlg;
namespace Ui {
class CLampHistoryDataDialog;
}

class CLampHistoryDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLampHistoryDataDialog(QWidget *parent = 0);
    ~CLampHistoryDataDialog();
    void getLampSerialNumber(QString lampID);
private:
    Ui::CLampHistoryDataDialog *ui;

    CHistoryEnvDataDlg* m_historyEnvDlg;
    QString m_lampSerialNum;
    CSSLampHistorydataBase* m_LampHistoryServer;

private:
    QString convertCHLampEnvDataToEn(QString chEnvData);
    QVariantList getHistoryEnvData(QDateTime startT, QDateTime endT, QString lampID, QString strEnv);
    QString prepareHistoryHourEnvData(QVariantList list);

private Q_SLOTS:
    void onViewButtonClicked();
};

#endif // CLAMPHISTORYDATADIALOG_H
