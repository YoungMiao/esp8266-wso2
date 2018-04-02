#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = 0);
    ~LoginPage();

private:
    Ui::LoginPage *ui;
    QString m_username;
    QString m_password;

    MainWindow* m_mainWindow;
    QVector<lampInfoStru>  m_lampInfoVecVec;

 private:

    void resizeEvent(QResizeEvent *event);
    QString getRandom();

 private slots:
    void onLoginButtonClicked();
};

#endif // LOGINPAGE_H
