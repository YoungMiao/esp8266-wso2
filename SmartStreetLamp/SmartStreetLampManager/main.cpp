
#include <QApplication>
#include "init\loginpage.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(QString::fromWCharArray(L"智慧路灯管理系统"));
    RibbonStyle* ribbonStyle = new RibbonStyle;
    ribbonStyle->setTheme(OfficeStyle::Office2016Colorful);
    a.setStyle(ribbonStyle);

    LoginPage  *loginPage = new LoginPage();
    loginPage->show();

    return a.exec();
}
