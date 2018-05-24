#include "pointpolarstream.h"
#include "boardplot.h"
#include "pointpolardegreeseries.h"
#include <QMessageBox>
PointPolarStream::PointPolarStream(BoardPlot* owner)
    : data(new PointPolarDegreeSeries(QVector<QwtPointPolar>())), owner(owner), lineColor(Qt::black), pointColor(Qt::black)
{
    setType(StreamType::Lines);
    curve.setData(data);
    curve.attach(owner);
}

void PointPolarStream::setLineColor(QColor color)
{
    lineColor = color;
    if(type == PointPolarStream::Lines)
        curve.setPen( QPen( lineColor ) );
}

void PointPolarStream::setPointColor(QColor color)
{
    pointColor = color;
    if(type == PointPolarStream::Points)
        curve.setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( pointColor ), QPen( Qt::transparent ), QSize( 9, 9 ) ) );
}

void PointPolarStream::setType(PointPolarStream::StreamType type_)
{
    type = type_;
    if(type == PointPolarStream::Lines)
    {
        curve.setStyle( QwtPolarCurve::Lines );
        curve.setSymbol( new QwtSymbol() );
        curve.setPen( QPen( lineColor ) );
    }
    else if(type == PointPolarStream::Points)
    {
        curve.setStyle( QwtPolarCurve::NoCurve );
        curve.setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( pointColor ), QPen( Qt::transparent ), QSize( 9, 9 ) ) );
    }
    else if(type == PointPolarStream::Hidden)
    {
        curve.setStyle( QwtPolarCurve::NoCurve );
        curve.setSymbol( new QwtSymbol() );
    }
}

void PointPolarStream::clear()
{
    data->setSamples(QVector<QwtPointPolar>());
    owner->replot();
}

void PointPolarStream::append(QwtPointPolar sample)
{
    data->append(sample);
    owner->replot();
}

void PointPolarStream::setRawData(QVector<QwtPointPolar> raw)
{
    data->setSamples(raw);
    owner->replot();
}

QVector<QwtPointPolar> PointPolarStream::getRawData()
{
    return data->samples();
}

