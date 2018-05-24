#ifndef POINTPOLARSTREAM_H
#define POINTPOLARSTREAM_H

#include <qwt_polar_plot.h>
#include <qwt_polar_curve.h>
#include <qwt_symbol.h>

class PointPolarDegreeSeries;
class BoardPlot;
class PointPolarStream
{
    friend class BoardPlot;
public:
    enum StreamType
    {
        Points,
        Lines,
        Hidden,
    };
private:
    PointPolarDegreeSeries* data;
    BoardPlot* owner;
    QwtPolarCurve curve;
    StreamType type;
    PointPolarStream(BoardPlot* owner);
    QColor lineColor;
    QColor pointColor;
public:
    void setLineColor( QColor color );
    void setPointColor( QColor color );
    void setType( StreamType type );
    void clear();
    void append(QwtPointPolar sample);
    void setRawData(QVector<QwtPointPolar> raw);
    QVector<QwtPointPolar> getRawData();
};


#endif // POINTPOLARSTREAM_H
