#include "pointstream.h"
#define SIDEPLOT_WINDOW_COUNT 50

PointStream::PointStream(QwtPlot* owner)
    : data(new QwtPointSeriesData(QVector<QPointF>())), owner(owner), lineColor(Qt::black)
{
    setType(StreamType::Lines);
    curve.setData(data);
    curve.attach(owner);
    clear();
}

void PointStream::setLineColor(QColor color)
{
    lineColor = color;
    if(type == PointStream::Lines)
        curve.setPen( QPen( lineColor ) );
}

void PointStream::setType(PointStream::StreamType type_)
{
    type = type_;
    if(type == PointStream::Lines)
    {
        curve.setStyle( QwtPlotCurve::Lines );
        curve.setSymbol( new QwtSymbol() );
        curve.setPen( QPen( lineColor ) );
    }
    else if(type == PointStream::Hidden)
    {
        curve.setStyle( QwtPlotCurve::NoCurve );
        curve.setSymbol( new QwtSymbol() );
    }
}

void PointStream::clear()
{
    data->setSamples(QVector<QPointF>());
    owner->setAxisScale( QwtPlot::xBottom, 1, SIDEPLOT_WINDOW_COUNT );
    owner->replot();
}

void PointStream::append(QPointF sample)
{
    QVector<QPointF> samples = data->samples();
    samples.append(sample);
    if( samples.size() > SIDEPLOT_WINDOW_COUNT )
    {
        owner->setAxisScale( QwtPlot::xBottom, samples[ samples.size() - SIDEPLOT_WINDOW_COUNT ].x(), samples[ samples.size() - 1 ].x() );
        QVector<QPointF> copy;
        for(int i = samples.size() - SIDEPLOT_WINDOW_COUNT; i < samples.size(); i++)
            copy.append(samples[i]);
        data->setSamples(copy);
    }
    else
    {
        data->setSamples(samples);
    }

    owner->replot();
}

void PointStream::setRawData(QVector<QPointF> raw)
{
    data->setSamples(raw);
    owner->replot();
}

QVector<QPointF> PointStream::getRawData()
{
    return data->samples();
}
