#include "echartswebview.h"
#include <memory>
#include <QEventLoop>
#include <QTimer>
#include <QWebChannel>

QVariant syncRunJavaScript(QWebEnginePage *page, const QString &javascript, int msec = 3000)
{
    QVariant result;
    std::shared_ptr<QEventLoop> loop = std::make_shared<QEventLoop>();
    QTimer::singleShot(msec, loop.get(), &QEventLoop::quit);
    page->runJavaScript(javascript, [loop, &result](const QVariant &val) {
        if (loop->isRunning()) {
            result = val;
            loop->quit();
        }
    });
    loop->exec();
    return result;
}

QString transQColor2String(const QColor& clr)
{
    int nRed = clr.red();
    int nGreen = clr.green();
    int nBlue = clr.blue();
    int nAlpha = clr.alpha();
    QString strRed = QString::number(nRed,16);
    if(nRed < 16)
        strRed = QString("0")+strRed;
    QString strGreen = QString::number(nGreen,16);
    if(nGreen < 16)
        strGreen = QString("0")+strGreen;
    QString strBlue = QString::number(nBlue,16);
    if(nBlue < 16)
        strBlue = QString("0")+strBlue;
    QString strAlpha = QString::number(nAlpha,16);
    if(nAlpha < 16)
        strAlpha = QString("0")+strAlpha;
    return QString("#") + strRed + strGreen + strBlue;
}

EchartsWebView::EchartsWebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_isLoaded = false;
    QWebChannel *channel = new QWebChannel(this);
    page()->setWebChannel(channel);
    connect(this,&EchartsWebView::urlChanged,[this](){m_isLoaded = false;});
    connect(this,&EchartsWebView::loadFinished,[this](bool b){m_isLoaded = b;});
    setContextMenuPolicy(Qt::NoContextMenu);
}

void EchartsWebView::runJavaScript(const QString &javascript)
{
//    syncRunJavaScript(page(),javascript);
    page()->runJavaScript(javascript);
}

void EchartsWebView::registerObject(const QString &id, QObject *obj)
{
    if(page()->webChannel())
    {
        page()->webChannel()->registerObject(id,obj);
    }
}

void EchartsWebView::deregisterObject(QObject *object)
{
    if(page()->webChannel())
    {
        page()->webChannel()->deregisterObject(object);
    }
}

void EchartsWebView::showOrHideSeries(const QString &name, bool isShow)
{
    runJavaScript(QString("showOrHideSeries('")+name+"',"+(isShow?"1":"0")+")");
}

void EchartsWebView::setHTMLBackground(const QColor &clr)
{
    QString strColor = transQColor2String(clr);
    runJavaScript(QString("setHTMLbackground('")+strColor+"')");
}

void EchartsWebView::setEchartsBackground(const QColor &clr)
{
    QString strColor = transQColor2String(clr);
    runJavaScript(QString("setEChartsbackground('")+strColor+"')");
}

void EchartsWebView::setAxisTextColor(const QColor &clr)
{
    QString strColor = transQColor2String(clr);
    runJavaScript(QString("setAxisTextColor('")+strColor+"')");
}

void EchartsWebView::setAxisLineColor(const QColor &clr)
{
    QString strColor = transQColor2String(clr);
    runJavaScript(QString("setAxisLineColor('")+strColor+"')");
}


