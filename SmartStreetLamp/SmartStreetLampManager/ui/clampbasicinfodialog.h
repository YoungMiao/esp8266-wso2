#ifndef CLAMPBASICINFODIALOG_H
#define CLAMPBASICINFODIALOG_H

#include <QDialog>
#include "include/lampinfo.h"
namespace Ui {
class CLampBasicInfoDialog;
}
class QNetworkAccessManager;
class CLampBasicInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLampBasicInfoDialog(QWidget *parent = 0);
    ~CLampBasicInfoDialog();
private:
    // rewrite virtual function
    virtual void resizeEvent(QResizeEvent *event);
    void initLampBasicInfoTable();
private:
    Ui::CLampBasicInfoDialog *ui;

public:

    void setLampBasicInfo(lampInfoStru lampInfo);
};

#endif // CLAMPBASICINFODIALOG_H
