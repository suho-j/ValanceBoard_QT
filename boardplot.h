#ifndef BOARDPLOT_H
#define BOARDPLOT_H

#include <qwt_polar_plot.h>
#include <qwt_polar_marker.h>

#include "pointpolarstream.h"

class BoardPlot : public QwtPolarPlot
{
    Q_OBJECT
private:
    QwtPolarMarker* marker;
    virtual void drawCanvas(QPainter *, const QRectF &) const;
    QString centerText;

public:
    std::vector<PointPolarStream*> streams;
    BoardPlot( QWidget* = NULL );
    virtual ~BoardPlot();
    void addStream();
    void setMarkerPosition( QwtPointPolar pp );
    void showMarker();
    void hideMarker();
    void setCenterText(QString text);
};

#endif // BOARDPLOT_H
