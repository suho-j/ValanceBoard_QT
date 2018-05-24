#ifndef POINTSTREAM_H
#define POINTSTREAM_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_point_data.h>

class QwtPlot;
class PointStream
{
    friend class SidePlot;

public:
    enum StreamType
    {
        Lines,
        Hidden,
    };
private:
    QwtPointSeriesData* data;
    QwtPlot* owner;
    QwtPlotCurve curve;
    StreamType type;
    PointStream(QwtPlot* owner);
    QColor lineColor;
public:
    void setLineColor( QColor color );
    void setType( StreamType type );
    void clear();
    void append(QPointF sample);
    void setRawData(QVector<QPointF> raw);
    QVector<QPointF> getRawData();
};

#endif // POINTSTREAM_H
