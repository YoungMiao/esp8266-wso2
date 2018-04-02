#ifndef ECHARTSWEBVIEW_H
#define ECHARTSWEBVIEW_H

#include <QWebEngineView>

class EchartsWebView : public QWebEngineView
{
    Q_OBJECT
public:
    EchartsWebView(QWidget* parent = 0);

    bool isUrlLoaded() const
    {
        return m_isLoaded;
    }

    void runJavaScript(const QString&);
    void registerObject(const QString& ,QObject*);
    void deregisterObject(QObject *object);

    void showOrHideSeries(const QString&,bool);
    void setHTMLBackground(const QColor&);
    void setEchartsBackground(const QColor&);

    void setAxisTextColor(const QColor&);
    void setAxisLineColor(const QColor&);

private:
    bool m_isLoaded;    //网页是否加载完成
};

#endif // ECHARTSWEBVIEW_H
