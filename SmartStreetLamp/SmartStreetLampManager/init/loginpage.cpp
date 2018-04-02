#include "loginpage.h"
#include "ui_loginpage.h"

#include "component/clampdataservermanager.h"
#include <QJsonDocument>
#include <QMovie>
#include <QMessageBox>
#include <Qdebug>
LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    m_username = "";
    m_password = "";
    m_mainWindow = Q_NULLPTR;
    setWindowIcon(QIcon(":/resource/images/app.png"));
    setWindowTitle(QString::fromWCharArray(L"智慧路灯管理系统"));

    ui->label_loading->setWindowFlags(Qt::FramelessWindowHint);
    ui->label_loading->setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->pushButton, SIGNAL(clicked(bool)),this, SLOT(onLoginButtonClicked()));
    QString radomString = getRandom();
    qDebug()<<"radomString::"<<radomString;
    ui->lineEdit_verify->setText(radomString);

    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);
    this->setFixedSize(this->width(),this->height());
}

LoginPage::~LoginPage()
{
    delete ui;
}

QString LoginPage::getRandom()
{
    QString ret = "";
    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    for(int i=0; i<4; i++)
    {

        int c = (qrand() % 2) ? 'a' : 'A';
        ret += static_cast<QChar>(c + qrand() % 26);
    }
    return ret;
}


void LoginPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPixmap pixmap(QString(":/resource/login.png"));
    QPixmap newPix = pixmap.scaled(ui->widget->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    ui->widget->setAutoFillBackground(true);

    QPalette palette = ui->widget->palette();
    palette.setBrush(QPalette::Window, QBrush(newPix));
    ui->widget->setPalette(palette);
}


void LoginPage::onLoginButtonClicked()
{
    QString user = ui->lineEdit_user->text();
    QString passwd = ui->lineEdit_passwd->text();
    QString verify = ui->lineEdit_checkin->text();
    QString checkin = ui->lineEdit_verify->text();
//    if(verify != checkin)
//    {
//        QMessageBox::information(this,"",QString::fromWCharArray(L"验证码错误"),
//                                 QMessageBox::No);
//        ui->lineEdit_checkin->setText("");
//        ui->lineEdit_verify->setText(getRandom());
//        return;
//    }

    int userID = -1;
    bool matchFlag = gDataServerConfig->checkInputUsername(user, passwd, userID);
    if(!matchFlag)
    {
        QMessageBox::information(this, "",QString::fromWCharArray(L"用户名或密码错误"),
                                 QMessageBox::No);
        ui->lineEdit_user->setText("");
        ui->lineEdit_passwd->setText("");
        ui->lineEdit_checkin->setText("");
        ui->lineEdit_verify->setText(getRandom());
        return;
    }

    QMovie *movie = new QMovie(":/resource/loading.gif");
    ui->label_loading->setMovie(movie);
    movie->start();

    m_lampInfoVecVec = gDataServerConfig->getLampInfoByTableName(NORTH_SECOND_RING);
    movie->stop();
    if(m_mainWindow == Q_NULLPTR)
    {
        this->setHidden(true);
        m_mainWindow = new MainWindow(0, m_lampInfoVecVec);
        m_mainWindow->show();
    }
}

