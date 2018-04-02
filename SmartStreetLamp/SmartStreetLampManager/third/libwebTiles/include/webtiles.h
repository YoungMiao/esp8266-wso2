#ifndef WEBTILES_H
#define WEBTILES_H

#include "webtiles_global.h"
#include <QNetworkAccessManager>
#include <QFile>


class QLibrary;
class WEBTILESSHARED_EXPORT WebTiles : public QObject
{
    Q_OBJECT

signals:
    void addWebMap( const QString &path );

public:
    WebTiles();
    ~WebTiles();

    enum webTypes{
        GOOGLEMAP = 0,
        GOOGLESITE = 1,
    };

    void initWebTypeAndPath( webTypes type, const QString &path );
    bool scaleToZoom( double scale, double &destScale );
    void resetMapCenterAndExtent( double xcenter, double ycenter, double xmin, double xmax, double ymin, double ymax );

private slots:
    void OnGoogleMapReady();
private :
    QNetworkAccessManager *manager;
    webTypes mWebtype;
    QString mapPath;
    QLibrary *lib;
    double mGoogleOffsetX;
    double mGoogleOffsetY;
    double mXMax;
    double mXMin;
    double mYMax;
    double mYMin;
    int currentZoom;

    void fetchGoogleMapData(int z, int x, int y, webTypes type);
    void refreshGoogleMaps();
    void writeWorldFile(QFile &prj, double lon1, double lat1, double lon2, double lat2);
};

#endif // WEBTILES_H
