#ifndef CLIGHTTINGSTRATEGYDIALOG_H
#define CLIGHTTINGSTRATEGYDIALOG_H

#include <QDialog>
#include <QTime>
#include "include/lampinfo.h"
#include <QAbstractButton>
class CSmartStreetControlBase;
class QTreeWidgetItem;
namespace Ui {
class CLightingStrategyDialog;
}

class CLightingStrategyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLightingStrategyDialog(QWidget *parent = 0);
    ~CLightingStrategyDialog();

private:
    Ui::CLightingStrategyDialog *ui;

    QTime m_startTime;
    QTime m_endTime;
    QVector<lampInfoStru> m_lampInfoVec;

    CSmartStreetControlBase*  m_streetControl;

private:
    void initPara();
    QStringList getDeviceIdList();
    QTreeWidgetItem* addTreeWidgetItem(QTreeWidgetItem* item, QStringList strList);

    void setChildCheckState(QTreeWidgetItem* item, Qt::CheckState cs);
    void setParentCheckState(QTreeWidgetItem* itemParent);
    bool isTopLevelItem(QTreeWidgetItem* item);

    void getAllSelectedItem(QStringList& serialNumList);

private Q_SLOTS:
    void onRadioButtonClicked(QAbstractButton* button);

    void onLightingOnButtonClicked();
    void onLightingOffButtonClicked();
    //设置树的三态操作
    void onItemChangedHandle(QTreeWidgetItem*, int);
public:
    void initDisplayTree(QVector<lampInfoStru> lampInfoVec);
};

#endif // CLIGHTTINGSTRATEGYDIALOG_H
