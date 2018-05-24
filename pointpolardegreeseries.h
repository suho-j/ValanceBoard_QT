#ifndef POINTPOLARSERIES_H
#define POINTPOLARSERIES_H
#include <qwt_series_data.h>

class PointPolarDegreeSeries : public QwtArraySeriesData<QwtPointPolar>
{
public:
    static QwtPointPolar fromRadian(QwtPointPolar pp);
    static QwtPointPolar toRadian(QwtPointPolar pp);
    PointPolarDegreeSeries(QVector<QwtPointPolar> samples);
    void setSamples(QVector<QwtPointPolar> samples);
    void append(QwtPointPolar sample);
    const QVector<QwtPointPolar> samples();
    QRectF boundingRect() const;
};


#endif // POINTPOLARSERIES_H
